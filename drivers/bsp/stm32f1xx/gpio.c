#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include "bsp/gpio.h"
#include "system/init.h"
#include "common/inc/errno.h"
#include "common/inc/array.h"

LIST_HEAD(gpio_chips);

struct stm32_gpio_chip {
    struct gpio_chip gc;
    void *base;
};

void *gpiochip_get_data(struct gpio_chip *gc) {
    return gc->data;
}

static int stm32f1xx_gpio_get(struct gpio_chip *chip, unsigned int offset, int *value) {
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    *value = HAL_GPIO_ReadPin(gpio->base, 1 << offset);
    return ERR_NONE;
}

static int stm32f1xx_gpio_set(struct gpio_chip *chip, unsigned int offset, int value) {
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    HAL_GPIO_WritePin(gpio->base, 1 << offset, value);
    return ERR_NONE;
}

static int stm32f1xx_gpio_set_multiple(struct gpio_chip *chip, unsigned offset, unsigned mask, int value) {
    (void) offset;
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    unsigned int i;

    for (i = 0; i < 32; i++) {
        unsigned pin = (1 << i);
        if (mask & pin) {
            // Set the corresponding GPIO pin
            HAL_GPIO_WritePin(gpio->base, 1 << i, value);
        }
    }

    return ERR_NONE;
}

static int stm32f1xx_gpio_get_direction(struct gpio_chip *chip, unsigned int offset, int *direction) {
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    GPIO_TypeDef *GPIOx = gpio->base;
    uint8_t shift = (offset % 8) * 4;
    uint8_t mode;

    if (offset < 8) {
        mode = (GPIOx->CRL >> shift) & 0xF;
    } else {
        mode = (GPIOx->CRH >> shift) & 0xF;
    }

    if (mode < 0x03) *direction = GPIO_MODE_FLAGS_INPUT;
    else if (mode < 0x10) *direction = GPIO_MODE_FLAGS_OUTPUT;
    else *direction = GPIO_MODE_FLAGS_INIT;
    return mode;
}

static int stm32f1xx_gpio_config(struct gpio_chip *chip, unsigned int offset, struct gpio_config *cfg) {
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    GPIO_InitTypeDef init = {
        .Pin = cfg->pin,
        .Mode = cfg->mode,
        .Pull = cfg->pull,
        .Speed = cfg->speed
    };
    HAL_GPIO_Init(gpio->base, &init);

    return ERR_NONE;
}

static int stm32f1xx_gpio_enable_clock(struct gpio_chip *chip, unsigned int offset) {
    (void) offset;
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    if (gpio->base == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (gpio->base == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (gpio->base == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (gpio->base == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (gpio->base == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }

    return ERR_NONE;
}

static int stm32f1xx_gpio_disable_clock(struct gpio_chip *chip, unsigned int offset) {
    (void) offset;
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    if (gpio->base == GPIOA) {
        __HAL_RCC_GPIOA_CLK_DISABLE();
    } else if (gpio->base == GPIOB) {
        __HAL_RCC_GPIOA_CLK_DISABLE();
    } else if (gpio->base == GPIOC) {
        __HAL_RCC_GPIOA_CLK_DISABLE();
    } else if (gpio->base == GPIOD) {
        __HAL_RCC_GPIOA_CLK_DISABLE();
    } else if (gpio->base == GPIOE) {
        __HAL_RCC_GPIOE_CLK_DISABLE();
    }

    return ERR_NONE;
}

static int stm32f1xx_gpio_request(struct gpio_chip *chip, unsigned int offset, unsigned int cmd) {
    switch (cmd) {
        case GPIO_CMD_DISABLE_CLK:
            stm32f1xx_gpio_disable_clock(chip, offset);
            break;
        case GPIO_CMD_ENABLE_CLK:
            stm32f1xx_gpio_enable_clock(chip, offset);
            break;
        default:
            break;
    }

    return ERR_NONE;
}

//static struct gpio_chip chip = {
//    .label = "stm32f1xx_gpioa",
//    .request = stm32f1xx_gpio_request,
//    .get_direction = stm32f1xx_gpio_get_direction,
//    .config = stm32f1xx_gpio_config,
//    .get = stm32f1xx_gpio_get,
//    .set = stm32f1xx_gpio_set,
//    .set_multiple = NULL,
//    .base = 0,
//    .ngpio = 16
//};

void gpio_chip_probe(const struct gpio_chip_desc *desc) {
    struct stm32_gpio_chip *chip = malloc(sizeof(struct stm32_gpio_chip));

    if (!chip) {
        return;
    }
    memset(chip, 0, sizeof(struct stm32_gpio_chip));
    chip->base = desc->base_addr;

    chip->gc.base = 0;
    chip->gc.ngpio = desc->ngpio;
    chip->gc.label = desc->label;
    chip->gc.name = desc->name;
    chip->gc.request = stm32f1xx_gpio_request;
    chip->gc.get_direction = stm32f1xx_gpio_get_direction;
    chip->gc.config = stm32f1xx_gpio_config;
    chip->gc.get = stm32f1xx_gpio_get;
    chip->gc.set = stm32f1xx_gpio_set;
    chip->gc.set_multiple = stm32f1xx_gpio_set_multiple;
    chip->gc.data = chip;

    list_add_tail(&chip->gc.list, &gpio_chips);
}

void stm32f1xx_gpio_chip_remove(const char *label) {
    struct gpio_chip *gc = find_gpio_chip_by_label(label);
    struct  stm32_gpio_chip *chip;
    if (gc) {
        chip = (struct  stm32_gpio_chip *) gc->data;
        gpio_chip_close(gc);
        list_del(&gc->list);
        free(chip);
        chip = NULL;
    }
}

int stm32f1xx_gpio_init() {
    for (int i=0; i < ARRAY_SIZE(gpio_chip_table); i++) {
        gpio_chip_probe(&gpio_chip_table[i]);
    }

    return 0;
}

__initcall(stm32f1xx_gpio_init);

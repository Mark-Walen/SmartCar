#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include "bsp/inc/gpio.h"
#include "common/inc/errno.h"

struct stm32_gpio_chip {
    struct gpio_chip gc;
    void __iomem *base;
};

void *gpiochip_get_data(struct gpio_chip *gc) {
    return gc->gpiodev->data;
}

static int stm32f1xx_gpio_get(struct gpio_chip *chip, unsigned int offset, int *value) {
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    return HAL_GPIO_ReadPin((GPIO_TypeDef *) gpio->base, offset);
}

static int stm32f1xx_gpio_set(struct gpio_chip *chip, unsigned int offset, int value) {
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
    HAL_GPIO_WritePin(gpio->base, offset, value);
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

    if (mode < 0x03) *direction = GPIO_MODE_INPUT;
    else if (mode < 0x10) *direction = GPIO_MODE_OUTPUT;
    else *direction = GPIO_MODE_INIT;
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
    struct stm32_gpio_chip *gpio = gpiochip_get_data(chip);
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

static struct gpio_chip chip = {
    .label = "stm32f1xx_gpioa",
    .request = stm32f1xx_gpio_request,
    .get_direction = stm32f1xx_gpio_get_direction,
    .config = stm32f1xx_gpio_config,
    .get = stm32f1xx_gpio_get,
    .set = stm32f1xx_gpio_set,
    .base = 0,
    .ngpio = 16,
};

static int stm32f1xx_gpio_probe(void) {
    struct stm32_gpio_chip *gpio = {0};
    gpio->gc = chip;
    gpio->gc.parent = NULL;
    gpio->base = GPIOA;

    // register device gpio a to platform device
    chip.label = "stm32f1xx_gpiob";
    gpio->gc.parent = NULL;
    gpio->base = GPIOB;


    return ERR_NONE;
}

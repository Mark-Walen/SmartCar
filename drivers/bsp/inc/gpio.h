#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_MODE_INPUT         0x0
#define GPIO_MODE_OUTPUT        0x1
#define GPIO_MODE_INIT          0x2

#define GPIO_CMD_ENABLE_CLK             0x00
#define GPIO_CMD_DISABLE_CLK            0x01
#define GPIO_CMD_INTR_RISING            0x02

struct gpio_config
{
    uint32_t pin;
    uint32_t mode;
    uint32_t pull;
    uint32_t speed;
    uint32_t intr_type;
};

struct gpio_desc {
    struct gpio_device *gdev;
    uint32_t flags;
/* flag symbols are bit numbers */
#define FLAG_REQUESTED	            0
#define FLAG_IS_OUT	                1
#define FLAG_EXPORT	                2	/* protected by sysfs_lock */
#define FLAG_SYSFS	                3	/* exported via /sys/class/gpio/control */
#define FLAG_ACTIVE_LOW	            6	/* value has active low */
#define FLAG_OPEN_DRAIN	            7	/* Gpio is open drain type */
#define FLAG_OPEN_SOURCE            8	/* Gpio is open source type */
#define FLAG_USED_AS_IRQ            9	/* GPIO is connected to an IRQ */
#define FLAG_IS_HOGGED	            11	/* GPIO is hogged */
#define FLAG_SLEEP_MAY_LOOSE_VALUE  12	/* GPIO may loose value in sleep */
    const char *label;
    const char *name;
};

struct gpio_device
{
    device_t *dev;
    int id;
    const struct gpio_chip *chip;
    const struct gpio_desc *desc;
    volatile uint32_t base;
    uint16_t ngpio;         /* 该GPIO控制器上GPIO引脚的数量，等于descs数组的大小 */
    char *label;
    void *data;

    struct list_head list;
};

struct gpio_chip
{
    const char *label;
    struct gpio_device *gpiodev;
    struct device *parent;

    int (*request)(struct gpio_chip *chip, unsigned offset, unsigned int cmd);
    int (*free)(struct gpio_chip *chip, unsigned offset);
    int (*get_direction)(struct gpio_chip *chip, unsigned offset, int *value);
    int (*config)(struct gpio_chip *chip, unsigned offset, struct gpio_config *config);
    int (*get)(struct gpio_chip *chip, unsigned offset);
    int (*set)(struct gpio_chip *chip, unsigned offset, int value);
    void (*set_multiple)(struct gpio_chip *chip, unsigned offset, unsigned mask, int value);
    int base;
    uint16_t ngpio;
    const char *const *names;
};

#ifdef __cplusplus
}
#endif

#endif

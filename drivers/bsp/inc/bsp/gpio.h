#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_MODE_FLAGS_INPUT         0x0
#define GPIO_MODE_FLAGS_OUTPUT        0x1
#define GPIO_MODE_FLAGS_INIT          0x2

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
    const char *chip_name;
    const char *chip_label;
    const char *label;
    const char *name;
    uint32_t gpio;
};

struct gpio_line {
    const char *label;
    const char *name;
    uint32_t gpio;
    int flag;

    struct gpio_chip *chip;
};

/**
 * <h3>GPIO controller structure.</h3>
 * <p>If you have N GPIO ports, define a new structure with the base address
 * and create N instances of `gpio_chip`, one for each port.</p>
 *
 * <p><b>Example</b>:
 * <pre><code>
 * struct my_gpio {
 *     uintptr_t base_addr;
 *     struct gpio_chip chip;
 * };
 *
 * struct my_gpio gpio1 = { .base_addr = 0x40020000, .chip = {   } };
 * struct my_gpio gpio2 = { .base_addr = 0x40021000, .chip = {   } };</code></pre></p>
 */
struct gpio_chip
{
    const char *label;
    const char *name;
    void *data;
    int base;
    uint16_t ngpio;
    const char *const *names;

    /**
     * Request control of a GPIO pin.
     *
     * @param chip   [in]  Pointer to the GPIO chip instance.
     * @param offset [in]  GPIO pin offset, must be in the range [base, ngpios).
     * @param cmd    [in]  Command for the GPIO request (e.g., configure as input/output).
     *
     * @return 0 on success, errno on failure.
     */
    int (*request)(struct gpio_chip *chip, unsigned offset, unsigned int cmd);
    int (*free)(struct gpio_chip *chip, unsigned offset);
    int (*get_direction)(struct gpio_chip *chip, unsigned offset, int *value);
    int (*config)(struct gpio_chip *chip, unsigned offset, struct gpio_config *config);
    int (*get)(struct gpio_chip *chip, unsigned offset, int *value);
    int (*set)(struct gpio_chip *chip, unsigned offset, int value);
    void (*set_multiple)(struct gpio_chip *chip, unsigned offset, unsigned mask, int value);

    struct list_head list;
};

struct gpio_chip* gpio_chip_open_by_label(const char *label);
struct gpio_chip* gpio_chip_open_by_name(const char *name);
void gpio_chip_close(struct gpio_chip *chip);

struct gpio_line *gpio_chip_get_line(struct gpio_chip *chip, unsigned offset);

extern struct list_head gpio_chips;
#ifdef __cplusplus
}
#endif

#endif

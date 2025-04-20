#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_MODE_FLAGS_INPUT           0x0
#define GPIO_MODE_FLAGS_OUTPUT          0x1
#define GPIO_MODE_FLAGS_INIT            0x2

#define GPIO_CMD_ENABLE_CLK             0x00
#define GPIO_CMD_DISABLE_CLK            0x01
#define GPIO_CMD_INTR_RISING            0x02

#define GPIO_CHIP_NUM                   3
#define GPIO_NUM                        2

struct gpio_config
{
    uint32_t pin;
    uint32_t mode;
    uint32_t pull;
    uint32_t speed;
    uint32_t intr_type;
};

struct gpio_chip_desc {
    const char *label;
    const char *name;
    void *base_addr;
    int base;
    uint16_t ngpio;
};

// For init
struct gpio_desc {
    // gpio chip desc
    const char *chip_label;
    const char *chip_name;
    const uint32_t base_addr;

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
    struct list_head node;
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
    int (*set_multiple)(struct gpio_chip *chip, unsigned offset, unsigned mask, int value);

    struct list_head list;
    struct list_head lines;
};
struct gpio_chip * find_gpio_chip_by_name(const char *name);
struct gpio_chip * find_gpio_chip_by_label(const char *label);
struct gpio_chip* gpio_chip_open_by_label(const char *label);
struct gpio_chip* gpio_chip_open_by_name(const char *name);
void gpio_chip_close(struct gpio_chip *chip);

struct gpio_line *gpio_chip_get_line(struct gpio_chip *chip, unsigned offset);

extern struct list_head gpio_chips;
extern struct gpio_chip_desc gpio_chip_table[GPIO_CHIP_NUM];
extern struct gpio_desc gpio_table[GPIO_NUM];
#ifdef __cplusplus
}
#endif

#endif

//
// Created by Mark-Walen on 2025/3/11.
//
#include <stdlib.h>
#include <string.h>

#include "bsp/gpio.h"

static struct gpio_chip * find_uart_port_by_name(const char *name) {
    struct gpio_chip *chip, *tmp;

    list_for_each_entry_safe(chip, tmp, &gpio_chips, list) {
        if (strcmp(chip->name, name) == 0) {
            return chip;
        }
    }

    return NULL;
}

static struct gpio_chip * find_uart_port_by_label(const char *label) {
    struct gpio_chip *chip, *tmp;

    list_for_each_entry_safe(chip, tmp, &gpio_chips, list) {
        if (strcmp(chip->label, label) == 0) {
            return chip;
        }
    }

    return NULL;
}

struct gpio_chip* gpio_chip_open_by_label(const char *label) {
    struct gpio_chip *chip = find_uart_port_by_label(label);

    // some operations.

    return chip;
}

struct gpio_chip* gpio_chip_open_by_name(const char *name) {
    struct gpio_chip *chip = find_uart_port_by_name(name);

    // some operations.

    return chip;
}

void gpio_chip_close(struct gpio_chip *chip) {
    // ...
}

struct gpio_line *gpio_chip_get_line(struct gpio_chip *chip, unsigned offset) {
    struct gpio_line *line = malloc(sizeof(struct gpio_line));
    if (line == NULL) {
        return NULL;
    }
    memset(line, 0, sizeof(struct gpio_line));
    chip->get(chip, offset, &line->flag);
    return line;
}

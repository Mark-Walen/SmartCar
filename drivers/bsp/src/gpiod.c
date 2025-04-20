//
// Created by Mark-Walen on 2025/3/11.
//
#include <stdlib.h>
#include <string.h>

#include "bsp/gpio.h"

struct gpio_chip * find_gpio_chip_by_name(const char *name) {
    struct gpio_chip *chip, *tmp;

    list_for_each_entry_safe(chip, tmp, &gpio_chips, list) {
        if (strcmp(chip->name, name) == 0) {
            return chip;
        }
    }

    return NULL;
}

struct gpio_chip * find_gpio_chip_by_label(const char *label) {
    struct gpio_chip *chip, *tmp;

    list_for_each_entry_safe(chip, tmp, &gpio_chips, list) {
        if (strcmp(chip->label, label) == 0) {
            return chip;
        }
    }

    return NULL;
}

struct gpio_chip* gpio_chip_open_by_label(const char *label) {
    struct gpio_chip *chip = find_gpio_chip_by_label(label);

    if (!chip) {
        return NULL; // Return NULL if no chip is found
    }

    // Initialize the list head for managing GPIO lines
    if (list_empty(&chip->lines)) {
        INIT_LIST_HEAD(&chip->lines);
    }

    return chip;
}

struct gpio_chip* gpio_chip_open_by_name(const char *name) {
    struct gpio_chip *chip = find_gpio_chip_by_name(name);

    // some operations.
    if (!chip) {
        return NULL; // Return NULL if no chip is found
    }

    // Initialize the list head for managing GPIO lines
    if (list_empty(&chip->lines)) {
        INIT_LIST_HEAD(&chip->lines);
    }

    return chip;
}

void gpio_chip_close(struct gpio_chip *chip) {
    struct gpio_line *line, *tmp;
    if (!chip) {
        return;
    }

    list_for_each_entry_safe(line, tmp, &chip->lines, node) {
        list_del(&line->node);
        // reset line state.
        // free allocated line.
        free(line);
    }
}

struct gpio_line *find_gpio_line_by_pin_num(struct gpio_chip *chip, unsigned offset) {
    struct gpio_line *slow = NULL, *fast = NULL, *pos = NULL;
    int step = 2;
    if (!chip || list_empty(&chip->lines)) {
        return NULL;
    }

    list_for_each_entry(fast, &chip->lines, node) {
        if (fast->gpio >= offset) {
            break;
        }
        slow = fast;
        for (int i = 0; i < step && fast->node.next != &chip->lines; i++) {
            fast = list_entry(fast->node.next, struct gpio_line, node);
        }
    }

    pos = slow ? slow : list_first_entry(&chip->lines, struct gpio_line, node);
    list_for_each_entry_from(pos, &chip->lines, node) {
        if (pos->gpio == offset) {
            return pos; // Found
        }
        if (pos->gpio > offset) {
            break; // Early exit
        }
    }

    return NULL;
}

struct gpio_line *gpio_chip_get_line(struct gpio_chip *chip, unsigned offset) {
    struct gpio_line *line, *pos;
    if (!chip) {
        return NULL;
    }
    line = find_gpio_line_by_pin_num(chip, offset);
    // not registered. init it and insert by offset in order.
    if (line) {
        return line;
    }
    line = malloc(sizeof(struct gpio_line));
    if (!line) {
        return NULL;
    }
    memset(line, 0, sizeof(struct gpio_line));
    line->gpio = offset;
    line->chip = chip;

    // Insert in sorted order
    list_for_each_entry(pos, &chip->lines, node) {
        if (pos->gpio > offset) {
            list_add_tail(&line->node, &pos->node); // Insert before the larger node
            return line;
        }
    }

    list_add_tail(&line->node, &chip->lines);
    return line;
}

//
// Created by Mark-Walen on 2025/3/10.
//
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "bsp/uart.h"
#include "system/init.h"
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

struct stm32_uart_port {
    struct uart_port port;
    UART_HandleTypeDef huart;
    void *membase; /* Memory-mapped base address of UART registers */
};

static LIST_HEAD(uart_ports);

static int parse_status(const int status) {
    switch (status) {
        case HAL_BUSY:
            return UART_ERROR_BUSY;
        case HAL_ERROR:
            return UART_ERROR_INVALID_ARG;
        case HAL_TIMEOUT:
            return UART_ERROR_TIMEOUT;
        case HAL_OK:
            return UART_ERROR_NONE;
        default:
            return UART_ERROR_UNKNOWN;
    }
}

static void *uart_get_data(struct uart_port *port) {
    return port->data;
}

static struct uart_port * find_uart_port_by_name(const char *name) {
    struct uart_port *uart, *tmp;

    list_for_each_entry_safe(uart, tmp, &uart_ports, list) {
        if (strcmp(uart->name, name) == 0) {
            return uart;
        }
    }

    return NULL;
}

static struct uart_port * find_uart_port_by_label(const char *label) {
    struct uart_port *uart, *tmp;

    list_for_each_entry(uart, &uart_ports, list) {
        if (strcmp(uart->label, label) == 0) {
            return uart;  // Found
        }
    }
    return NULL;  // Not found
}

static int stm32f1xx_uart_write(struct uart_port *port, uint8_t *buf, size_t len) {
    int status = 0;
    struct stm32_uart_port *uart = uart_get_data(port);
    if (port->dma_enabled)
        status = HAL_UART_Transmit_DMA(&uart->huart, buf, len);
    else {
        status = HAL_UART_Transmit(&uart->huart, buf, len, UART_RX_TIMEOUT);
    }

    return parse_status(status);
}

static int stm32f1xx_uart_read(struct uart_port *port, uint8_t *buf, size_t len) {
    int status = 0;
    struct stm32_uart_port *uart = uart_get_data(port);
    if (port->dma_enabled)
        status = HAL_UART_Receive_DMA(&uart->huart, buf, len);
    else {
        status = HAL_UART_Receive(&uart->huart, buf, len, UART_RX_TIMEOUT);
    }

    return parse_status(status);
}

static int stm32f1xx_uart_get_char(struct uart_port *port, uint8_t *data) {
    int status = 0;
    struct stm32_uart_port *uart = uart_get_data(port);
    if (port->dma_enabled)
        status = HAL_UART_Receive_DMA(&uart->huart, data, 1);
    else {
        status = HAL_UART_Receive(&uart->huart, data, 1, UART_RX_TIMEOUT);
    }

    return parse_status(status);
}

static int stm32f1xx_uart_put_char(struct uart_port *port, uint8_t data) {
    int status = 0;
    struct stm32_uart_port *uart = uart_get_data(port);
    if (port->dma_enabled)
        status = HAL_UART_Transmit_DMA(&uart->huart, &data, 1);
    else {
        status = HAL_UART_Transmit(&uart->huart, &data, 1, UART_TX_TIMEOUT);
    }

    return parse_status(status);
}

static int stm32f1xx_uart_config(struct uart_port *port, const struct uart_config *config) {
    int status = UART_ERROR_NONE;

    struct stm32_uart_port *uart = uart_get_data(port);
    UART_HandleTypeDef *huart = &uart->huart;

    /* Update huart configs */
    huart->Instance = uart->membase;
    huart->Init.BaudRate = config->baudrate;
    huart->Init.WordLength = config->word_length;
    huart->Init.StopBits = config->stop_bits;
    huart->Init.Parity = config->parity;
    huart->Init.Mode = config->mode;
    huart->Init.HwFlowCtl = config->flow_control;
    huart->Init.OverSampling = config->oversample;

    /* Update port DMA enable flag. */
    uart->port.dma_enabled = config->dma_enabled;

    status = HAL_UART_Init(&uart->huart);
    return parse_status(status);
}

// static int stm32f1xx_uart_flush(struct uart_port *port);
static int stm32f1xx_uart_set_baud(struct uart_port *port, uint32_t baudrate) {
    int status = 0;
    struct stm32_uart_port *uart = uart_get_data(port);
    UART_HandleTypeDef *huart = &uart->huart;

    huart->Init.BaudRate = baudrate;

    status = HAL_UART_Init(&uart->huart);
    return parse_status(status);
}

static int stm32f1xx_uart_set_mode(struct uart_port *port, uint32_t word_length, uint32_t parity,
                                   uint32_t stop_bits, uint32_t flow_control) {
    int status = 0;
    struct stm32_uart_port *uart = uart_get_data(port);
    UART_HandleTypeDef *huart = &uart->huart;

    // huart configs
    huart->Init.WordLength = word_length;
    huart->Init.Parity = parity;
    huart->Init.StopBits = stop_bits;
    huart->Init.HwFlowCtl = flow_control;

    status = HAL_UART_Init(&uart->huart);
    return parse_status(status);
}

void stm32_uart_probe(const struct uart_port_desc *port_desc) {
    struct stm32_uart_port *uart = malloc(sizeof(struct stm32_uart_port));
    struct gpio_chip *rx_chip = gpio_chip_open_by_name(port_desc->rx_chip_name);
    struct gpio_chip *tx_chip = gpio_chip_open_by_name(port_desc->tx_chip_name);
    struct uart_ops ops = {0};
    if (!uart) {
        return;  // Handle allocation failure
    }

    // throw an exception if needed.
    // if (!rx_chip && !tx_chip) {
    //     return;
    // }

    // init
    ops.write = stm32f1xx_uart_write;
    ops.read = stm32f1xx_uart_read;
    ops.get_char = stm32f1xx_uart_get_char;
    ops.put_char = stm32f1xx_uart_put_char;
    ops.config = stm32f1xx_uart_config;
    ops.set_baud = stm32f1xx_uart_set_baud;
    ops.set_mode = stm32f1xx_uart_set_mode;

    // Initialize UART instance
    uart->port.label = port_desc->label;
    uart->port.ops = &ops;
    uart->port.data = uart;
    uart->port.name = port_desc->name;
    uart->membase = port_desc->base_addr;
    uart->huart = (UART_HandleTypeDef){0};
    uart->port.gpio_tx = tx_chip ? gpio_chip_get_line(tx_chip, port_desc->gpio_tx) : NULL;
    uart->port.gpio_rx = rx_chip ? gpio_chip_get_line(rx_chip, port_desc->gpio_rx) : NULL;

    // Add to linked list
    list_add_tail(&uart->port.list, &uart_ports);
}

void stm32_uart_remove(const char *label) {
    struct uart_port *port = find_uart_port_by_label(label);
    struct stm32_uart_port *uart;
    if (port) {
        uart = (struct stm32_uart_port *) port->data;
        list_del(&port->list);
        free(uart);
        uart = NULL;
    }
}

int stm32f1xx_uart_init(void) {
    for (int i = 0; i < ARRAY_SIZE(uarts); i++) {
        stm32_uart_probe(&uarts[i]);
    }

    return 0;
}

__initcall(stm32f1xx_uart_init);

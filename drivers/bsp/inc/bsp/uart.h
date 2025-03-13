//
// Created by Mark-Walen on 2025/3/10.
//

#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include <lwrb/lwrb.h>

#include "bsp/gpio.h"

#ifndef UART_NUM
    #define UART_NUM 2
#endif

#ifndef UART_RX_BUFFER_SIZE
    #define UART_RX_BUFFER_SIZE 1024
#endif

#ifndef UART_TX_BUFFER_SIZE
    #define UART_TX_BUFFER_SIZE 1024
#endif

#ifndef UART_RX_TIMEOUT
    #define UART_RX_TIMEOUT 100
#endif

#ifndef UART_TX_TIMEOUT
    #define UART_TX_TIMEOUT 100
#endif

#ifndef UART_BAUDRATE
    #define UART_BAUD_RATE 115200
#endif
/**
 * \brief           UART PORT structure forward declaration
 */
struct uart_port;

/* UART Event Types */
enum uart_event {
    UART_EVENT_TX_DONE,    /* Transmission complete */
    UART_EVENT_RX_READY,   /* Data available for reading */
    UART_EVENT_ERROR,      /* Framing, parity, or overrun error */
    UART_EVENT_BREAK,      /* Break condition detected */
};

/* UART Error Codes */
enum uart_error {
    UART_ERROR_NONE = 0,       /* No error */
    UART_ERROR_UNKNOWN,
    UART_ERROR_INVALID_ARG,    /* Invalid argument */
    UART_ERROR_TIMEOUT,        /* Operation timed out */
    UART_ERROR_BUSY,
    UART_ERROR_HARDWARE,       /* Hardware failure */
    UART_ERROR_BUFFER_FULL,    /* Buffer is full */
    UART_ERROR_BUFFER_EMPTY,   /* Buffer is empty */
    UART_ERROR_NOT_SUPPORTED,  /* Operation not supported */
};

/* UART Configuration Structure */
struct uart_config {
    uint32_t baudrate;         /* Baud rate (e.g., 9600, 115200) */
    uint32_t word_length;      /* Number of data bits (e.g., 8) */
    uint32_t parity;           /* Parity mode (0: none, 1: odd, 2: even) */
    uint32_t stop_bits;        /* Number of stop bits (1 or 2) */
    uint32_t flow_control;     /* Flow control mode (0: none, 1: hardware) */
    uint32_t mode;
    uint32_t oversample;

    uint8_t dma_enabled:1;          /* Whether DMA is enabled (true/false) */

    struct gpio_config gpio_tx;
    struct gpio_config gpio_rx;
};

struct uart_ops {
    int (*write)(struct uart_port *port, uint8_t *buf, size_t len);
    int (*read)(struct uart_port *port, uint8_t *data, size_t len);
    int (*get_char)(struct uart_port *port, uint8_t *data);
    int (*put_char)(struct uart_port *port, uint8_t data);
    int (*config)(struct uart_port *port, const struct uart_config *config);
    int (*flush)(struct uart_port *port);
    int (*set_baud)(struct uart_port *port, uint32_t baudrate);
    int (*set_mode)(struct uart_port *port, uint32_t word_length, uint32_t parity,
                    uint32_t stop_bits, uint32_t flow_control);
};

/* UART Port Structure */
struct uart_port {
    const char *name;          /* Device name (e.g., "/dev/ttyUSB0") */
    const char *label;         /* Human-readable identifier for the UART port */
    void *data;                /* Device-specific data */

    const struct uart_ops *ops; /* UART operations */
    struct gpio_line *gpio_tx;  /* GPIO descriptor for TX pin */
    struct gpio_line *gpio_rx;  /* GPIO descriptor for RX pin */

    /* Ring buffers for RX and TX */
    uint8_t dma_enabled:1;          /* Whether DMA is enabled (true/false) */
    lwrb_t *tx, *rx;

    /* Event callback */
    void (*event_callback)(struct uart_port *port, enum uart_event event, void *arg);
    void *event_arg;           /* Optional argument for the event callback */

    /* IRQ handler (callback function) */
    void (*irq_handler)(struct uart_port *port);

    struct list_head list;
};

struct uart_port_desc {
    const char *name;
    const char *label;
    void *base_addr;

    const char *tx_chip_name;   /* TX gpio chip name */
    const char *rx_chip_name;   /* RX gpio chip name */

    const uint32_t gpio_tx;     /* TX gpio chip name */
    const uint32_t gpio_rx;
};

extern struct uart_port_desc uarts[UART_NUM];

struct uart_port *uart_port_open_by_name(const char *name);
struct uart_port *uart_port_open_by_label(const char *label);
void uart_port_close(struct uart_port *port);

int uart_config(const char *name, struct uart_config *config);
int uart_write(const char *name, uint8_t *buf, size_t len);
int uart_read(const char *name, uint8_t *data, size_t len);
int uart_get_char(const char *name, uint8_t *data);
int uart_put_char(const char *name, uint8_t data);
#endif //_UART_H

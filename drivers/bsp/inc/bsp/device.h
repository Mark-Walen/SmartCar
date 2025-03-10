#ifndef __BSP_DEVICE_H__
#define __BSP_DEVICE_H__

#include <stdint.h>
#include "common/inc/list.h"
#include "bus.h"

typedef uint32_t u32;
typedef u32 dev_t;

#ifdef __cplusplus
extern "C" {
#endif

#define __iomem __attribute__((noderef, address_space(2)))

/**
 * \brief           Device structure forward declaration
 */
struct device;
/**
 * The type of device, "struct device" is embedded in. A class
 * or bus can contain devices of different types
 * like "partitions" and "disks", "mouse" and "event".
 * This identifies the device type and carries type-specific
 * information, equivalent to the kobj_type of a kobject.
 * If "name" is specified, the uevent will contain it in
 * the DEVTYPE variable.
 */
typedef struct device_type {
    const char *name;

    void (*release)(void);
} device_type_t;

typedef struct pm_message {
    int event;
} pm_message_t;

typedef struct device_driver {
    const char *name;
    const struct bus_type *bus;

    int (*probe)(struct device *dev);

    void (*sync_state)(struct device *dev);

    int (*remove)(struct device *dev);

    void (*shutdown)(struct device *dev);

    int (*suspend)(struct device *dev, pm_message_t state);

    int (*resume)(struct device *dev);
} device_driver_t;

// Root structure of BSP peripheral.
typedef struct device {
    struct device *parent;
    struct bus_type *bus;
    struct device_driver *driver;
    const char *init_name;
    dev_t devt; /* dev_t, creates the sysfs "dev" */
    u32 id; /* device instance */
    void *private_data;
    struct list_head devres_head;
} device_t;

#ifdef __cplusplus
}
#endif

#endif

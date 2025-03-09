#ifndef _ERRNO_H
#define _ERRNO_H

enum errorno {
    ERR_NONE=0U,
    /** Common errors*/
    ERR_NULLPTR,

    /** System errors */
    ERR_SYS_INIT, // System init state error.

    /** Device driver errors */
    ERR_DRIVER_PROBE,
};

#endif

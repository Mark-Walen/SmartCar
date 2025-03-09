#ifndef _INIT_H
#define _INIT_H

#include <system/types.h>

typedef int (*initcall_t)(void);

#define __initcall(fn) \
  const initcall_t __initcall_##fn __attribute__((section("initcall"), used)) = fn;

#define early_initcall(fn) \
  const initcall_t __initcall_##fn __attribute__((section("early_initcall"), used)) = fn;

#define late_initcall(fn) \
  const initcall_t __initcall_##fn __attribute__((section("late_initcall"), used)) = fn;

#define platform_initcall(fn) \
  const initcall_t __initcall_##fn __attribute__((section("platform_initcall"), used)) = fn;

#define module_initcall(fn) \
  const initcall_t __initcall_##fn __attribute__((section("module_initcall"), used)) = fn;


#define __do_initcall(prefix) \
  do { \
    initcall_t *init; \
    extern initcall_t prefix##$$Base[]; \
    extern initcall_t prefix##$$Limit[]; \
    for (init = prefix##$$Base; init < prefix##$$Limit; ++init) { \
      if (*init) (*init)(); \
    } \
  } while (0)

#endif

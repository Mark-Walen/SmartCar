//
// Created by Mark-Walen on 2025/3/7.
//

#ifndef _TYPES_H
#define _TYPES_H

#if defined(__GNUC__)

///<locate code to x segment ever exist
#define SEC_USED(x)     __attribute__((section(#x),used))
///<locate code to x segment optimized by dependency
#define SEC(x)          __attribute__((section(#x)))
#define sec(x)          __attribute__((section(#x),used))
///<locate data to x segment
#define AT(x)           __attribute__((section(#x)))
#define SET(x)          __attribute__((x))
#define ALIGNED(x)	    __attribute__((aligned(x)))
#define _GNU_PACKED_	__attribute__((packed))
#define _NOINLINE_	    __attribute__((noinline))
#define _INLINE_	    __attribute__((always_inline))
#define _WEAK_	        __attribute__((weak))
#define _WEAKREF_	    __attribute__((weakref))
#define _NORETURN_      __attribute__((noreturn))
#define _NAKED_         __attribute__((naked))
#else

#define SEC_USED(x)
#define SEC(x)
#define AT(x)
#define SET(x)
#define ALIGNED(x)
#define _GNU_PACKED_
#define _NOINLINE_
#define _INLINE_
#define _WEAK_
#define _WEAKREF_
#define _NORETURN_
#define _NAKED_
#endif

#endif //_TYPES_H

/*
 * MIT License
 * Based on interpose.hh which is copyright (c) 2017 Charlie Curtsinger
 */

#if !defined(__INTERPOSE_H)
#define __INTERPOSE_H

#include <stdint.h>

#include <dlfcn.h>

#if defined(__ELF__)

#if !defined _GNU_SOURCE
# error Must define _GNU_SOURCE for RTLD_NEXT to be available.
#endif

// The C version of the interposition macro differs from that of the C++ version: the user
// has to provide the argument type-and-name list as a macro argument (instead of postfixing
// it to the macro invocation), and to redundantly provide the list of just argument names,
// which must match those in the type-and-name list.
#define INTERPOSE_C(RETURN_TYPE, NAME, ARG_TYPE_AND_NAME_LIST, ARG_NAME_LIST) \
  static RETURN_TYPE Real__##NAME ARG_TYPE_AND_NAME_LIST { \
    static __typeof__(NAME)* real_##NAME; \
    __typeof__(NAME)* func = __atomic_load_n(&real_##NAME, __ATOMIC_CONSUME); \
    if(!func) { \
      func = (__typeof__(NAME)*)( \
        (uintptr_t)(dlsym(RTLD_NEXT, #NAME))); \
      __atomic_store_n(&real_##NAME, func, __ATOMIC_RELEASE); \
    } \
    return func ARG_NAME_LIST; \
  } \
  extern __typeof__(NAME) NAME __attribute__((weak, alias("__interpose_" #NAME))); \
  extern RETURN_TYPE __interpose_##NAME ARG_TYPE_AND_NAME_LIST

#else
# error Unsupported platform.
#endif

#endif

#if !defined(__INTERPOSE_HH)
#define __INTERPOSE_HH

#include <functional>
#include <type_traits>
#include <dlfcn.h>

/// Function type inspection utility for interpose
template<typename F> struct fn_info {
  using type = F;
  using ret_type = void;
};

/// Specialize the fn_info template for functions with non-void return types
template<typename R, typename... Args> struct fn_info<R(Args...)> {
  using type = R(Args...);
  using ret_type = R;
};

#if defined(__linux__)

/**
 * The linux interposition process uses weak aliases to replace the original function
 * and creates a real::___ function that will perform dynamic symbol resolution on the
 * first call. Beware interposing on calloc: you must check for recursive calls to
 * calloc and return NULL because the dynamic linker calls calloc when attempting to
 * resolve the calloc symbol.
 */
#define INTERPOSE(NAME) \
  namespace real { \
    template<typename... Args> \
    auto NAME(Args... args) -> decltype(::NAME(args...)) { \
      static bool initialized = false; \
      static decltype(::NAME)* real_##NAME; \
      if(!initialized) { \
        fprintf(stderr ,"Resolving " #NAME "\n"); \
        real_##NAME = reinterpret_cast<decltype(::NAME)*>(dlsym(RTLD_NEXT, #NAME)); \
        initialized = true; \
      } \
      return real_##NAME(std::forward<Args>(args)...); \
    } \
  } \
  extern "C" decltype(::NAME) NAME __attribute__((weak, alias("__interpose_" #NAME))); \
  extern "C" fn_info<decltype(::NAME)>::ret_type __interpose_##NAME
          
#elif defined(__APPLE__)

/// Structure exposed to the linker for interposition
struct __osx_interpose {
	void *new_func;
	void *orig_func;
};

/// Shorthand to generate the OSX interpose struct
#define OSX_INTERPOSE_STRUCT(NEW, OLD) \
  static const __osx_interpose __osx_interpose_##OLD \
    __attribute__((used, section("__DATA, __interpose"))) = \
    { reinterpret_cast<void*>(NEW), reinterpret_cast<void*>(OLD) }

/**
  * The OSX interposition process is much simpler. Just create an OSX interpose struct,
  * include the actual function in the `real` namespace, and declare the beginning of the
  * replacement function with the appropriate return type.
  */
#define INTERPOSE(NAME) \
  namespace real { \
    using ::NAME; \
  } \
  extern "C" decltype(::NAME) __interpose_##NAME; \
  OSX_INTERPOSE_STRUCT(__interpose_##NAME, NAME); \
  extern "C" fn_info<decltype(::NAME)>::ret_type __interpose_##NAME

#endif

#endif

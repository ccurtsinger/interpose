# Library Call Interposition for Linux and Mac OS
This single-header project implements dynamic library symbol interposition for both macOS and Linux. This allows you to build shared libraries that can intercept calls to library functions for special handling or replacement. The syntax is the same for both Linux and Mac, despite the fact that these platforms support library interposition with very different mechanisms. This implementation makes use of C++11 features, so it will not work with older versions of C++.

To use the interpose tool, just `#include <interpose.hh>` in the source file where you will implement the replacement functions. A simple replacement function looks like this:

```
INTERPOSE(malloc)(size_t sz) {
  fprintf(stderr, "Caught a call to malloc(%lu)\n", sz);
  return real::malloc(sz);
}
```

This line declares a replacement function for `malloc` that logs the result, then passes the call on to the real `malloc` implementation. For this to work, the `malloc` symbol must be defined (by including `stdlib.h`). The replacement function is type-checked against the declared function, so compilation will fail if you try to replace `malloc` with a function that takes an `int` parameter. This helps prevent accidental type mismatch errors, but you can explicitly bypass this requirement by declaring `malloc` yourself instead of including the system-wide declaration.

There is an example library in `examples/logger` that logs calls to `malloc`, `free`, `calloc`, `realloc`, and `exit`. Go to this directory and run `make test` to build the interposition library and run it against a simple test program.

# Library Call Interposition for Linux and Mac OS
This single-header project implements dynamic library symbol interposition for both macOS and Linux. This allows you to build shared libraries that can intercept calls to library functions for special handling or replacement. The syntax is the same for both Linux and Mac, despite the fact that these platforms support library interposition with very different mechanisms. This implementation makes use of C++11 features, so it will not work with older versions of C++.

To use the interpose tool, just `#include <interpose.hh>` in the source file where you will implement the replacement functions. A simple replacement function looks like this:

```c++
INTERPOSE(malloc)(size_t sz) {
  fprintf(stderr, "Caught a call to malloc(%lu)\n", sz);
  return real::malloc(sz);
}
```

This line declares a replacement function for `malloc` that logs the result, then passes the call on to the real `malloc` implementation. For this to work, the `malloc` symbol must be defined (by including `stdlib.h`). The replacement function is type-checked against the declared function, so compilation will fail if you try to replace `malloc` with a function that takes an `int` parameter. This helps prevent accidental type mismatch errors, but you can explicitly bypass this requirement by declaring `malloc` yourself instead of including the system-wide declaration.

A C-only version is provided in `interpose.h`, differing in the name of the macro, the arguments passed to it, and the constructed name of the original function:

```c
INTERPOSE_C(void*, malloc, (size_t sz), (sz)) {
  fprintf(stderr, "Caught a call to malloc(%zu)\n", sz);
  return Real__malloc(sz);
}
```

There is an example library in `examples/logger` that intercepts calls to `malloc`, `free`, `calloc`, and `realloc`. This library tracks the number of bytes allocated and freed by the program, and prints allocation stats just before the program exits.

## Copyright & License
MIT License

Copyright (c) 2017 Charlie Curtsinger

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

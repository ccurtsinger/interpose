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

## Copyright & License
Copyright &copy; 2016 Charlie Curtsinger

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
# Interpose Example: Logger
This simple interposition example logs calls to `malloc`, `free`, `calloc`, `realloc`, and `exit`. The makefile also includes a simple test program that you can run with the logger preloaded by typing `make test`. The logger is implemented in [`logger.cc`](logger.cc).

The C version of the interposition macro is exercised by typing `make test_c`, with the corresponding logger being implemented in [`logger.c`](logger.c).

The logging code is fairly straightforward: each function is interposed-on with a replacement that logs the call and passes it along to the real implementation. The one complication is `calloc`, which includes a check for recursive calls. On Linux, `calloc` is called during dynamic library loading, but resolving the real `calloc` implementation will trigger this loading leading to an infinite recursive loop. However, if the replacement `calloc` returns `NULL` when called recursively, the dynamic linker will use an internal memory allocator instead.

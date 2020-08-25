#if !defined(__APPLE__)
# define _GNU_SOURCE
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <interpose.h>

#if defined(__APPLE__)
#include <malloc/malloc.h>
#define malloc_usable_size malloc_size
#elif defined(__linux__)
#include <malloc.h>
#endif

#define START_COLOR "\033[01;33m"
#define END_COLOR "\033[0m"

size_t allocated_bytes = 0;
size_t freed_bytes = 0;

INTERPOSE_C(void*, malloc, (size_t sz), (sz)) {
  void* result = Real__malloc(sz);
  allocated_bytes += malloc_usable_size(result);
  return result;
}

INTERPOSE_C_VOID(free, (void* p), (p)) {
  freed_bytes += malloc_usable_size(p);
  Real__free(p);
}

INTERPOSE_C(void*, calloc, (size_t n, size_t sz), (n, sz)) {
  void* result = Real__calloc(n, sz);
  allocated_bytes += malloc_usable_size(result);
  return result;
}

INTERPOSE_C(void*, realloc, (void* p, size_t sz), (p, sz)) {
  size_t old_size = malloc_usable_size(p);
  void* result = Real__realloc(p, sz);
  size_t new_size = malloc_usable_size(p);

  if(result == p) {
    if(new_size > old_size) allocated_bytes += new_size - old_size;
    else freed_bytes = old_size - new_size;
  } else {
    freed_bytes += old_size;
    allocated_bytes += new_size;
  }

  return result;
}

INTERPOSE_C_VOID(exit, (int status), (status)) {
  fprintf(stderr, START_COLOR);
  fprintf(stderr, "\n\nProgram Allocation Stats\n");
  fprintf(stderr, "  allocated %zu bytes\n", allocated_bytes);
  fprintf(stderr, "      freed %zu bytes\n", freed_bytes);
  if(allocated_bytes >= freed_bytes) {
    fprintf(stderr, "     leaked %zu bytes\n", allocated_bytes - freed_bytes);
  } else {
    fprintf(stderr, "      freed %zu extra bytes\n", freed_bytes - allocated_bytes);
  }
  fprintf(stderr, END_COLOR);

  Real__exit(status);
}

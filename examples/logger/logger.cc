#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <interpose.hh>

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

INTERPOSE(malloc)(size_t sz) {
  void* result = real::malloc(sz);
  allocated_bytes += malloc_usable_size(result);
  return result;
}

INTERPOSE(free)(void* p) {
  freed_bytes += malloc_usable_size(p);
  real::free(p);
}

INTERPOSE(calloc)(size_t n, size_t sz) {
  void* result = real::calloc(n, sz);
  allocated_bytes += malloc_usable_size(result);
  return result;
}

INTERPOSE(realloc)(void* p, size_t sz) {
  size_t old_size = malloc_usable_size(p);
  void* result = real::realloc(p, sz);
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

INTERPOSE(exit)(int status) {
  fprintf(stderr, START_COLOR);
  fprintf(stderr, "\n\nProgram Allocation Stats\n");
  fprintf(stderr, "  allocated %lu bytes\n", allocated_bytes);
  fprintf(stderr, "      freed %lu bytes\n", freed_bytes);
  if(allocated_bytes >= freed_bytes) {
    fprintf(stderr, "     leaked %lu bytes\n", allocated_bytes - freed_bytes);
  } else {
    fprintf(stderr, "      freed %lu extra bytes\n", freed_bytes - allocated_bytes);
  }
  fprintf(stderr, END_COLOR);
  
  real::exit(status);
}

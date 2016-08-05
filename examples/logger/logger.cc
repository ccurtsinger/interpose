#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <interpose.hh>

INTERPOSE(malloc)(size_t sz) {
  fprintf(stderr, "malloc(%lu)\n", sz);
  return real::malloc(sz);
}

INTERPOSE(free)(void* p) {
  fprintf(stderr, "free(%p)\n", p);
  return real::free(p);
}

INTERPOSE(calloc)(size_t n, size_t sz) {
  fprintf(stderr, "calloc(%lu, %lu)\n", n, sz);
  static __thread bool in_calloc = false;
  if(in_calloc) return NULL;
  in_calloc = true;
  void* result = real::calloc(n, sz);
  in_calloc = false;
  return result;
}

INTERPOSE(realloc)(void* p, size_t sz) {
  fprintf(stderr, "realloc(%p, %lu)\n", p, sz);
  return real::realloc(p, sz);
}

INTERPOSE(exit)(int status) {
  fprintf(stderr, "exit(%d)\n", status);
  real::exit(status);
}

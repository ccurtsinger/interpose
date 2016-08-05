#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
  printf("Hello\n");
  
  int* p = (int*)malloc(sizeof(int));
  printf("p is %p\n", p);
  free(p);
  
  exit(0);
}

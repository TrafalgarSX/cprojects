#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int a = 10;


  for (int i = 0; i < argc; ++i) {
    printf_s("%d\n", i);
    printf_s("%s\n", argv[i]);
  }

  printf_s("%d\n", a);
  printf_s("%s\n", "test1");
  printf_s("%s\n", "test2");
  return 0;
}

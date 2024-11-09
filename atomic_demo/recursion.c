#include <stdint.h>
#include <stdio.h>

void recursion(int n) {

  if (n > INT32_MAX - 1) {
    return;
  }
  printf("%d\n", n);
  recursion(n + 1);
}

int main(void) {

  recursion(0);
  return 0;

}

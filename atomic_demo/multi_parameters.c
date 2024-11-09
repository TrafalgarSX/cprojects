#include <stdarg.h>
#include <stdio.h>

int multi_parameters(int count, ...) {
  va_list ap;
  int sum = 0;
  va_start(ap, count);

  for (int i = 0; i < count; i++) {
    sum += va_arg(ap, int);
  }

  va_end(ap);
  return sum;
}

int multi_parameters_diff(int count, ...) {
  va_list ap;
  int sum = 0;
  va_start(ap, count);

  sum += va_arg(ap, int);
  printf("string = %s\n", va_arg(ap, char *));
  sum += va_arg(ap, int);

  va_end(ap);
  return sum;
}

int main(void) {
  int sum = 0;

  sum = multi_parameters_diff(3, 1, "test", 3);

  printf("sum = %d\n", sum);

  return 0;
}

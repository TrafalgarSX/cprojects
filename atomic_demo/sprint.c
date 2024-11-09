#include <stdio.h>
#include <string.h>

int main(void) {
  long long int number = 123456789;
  char buffer[12];

  sprintf(buffer, "%011lld", number);

  printf("%s\n", buffer);

  return 0x00;
}

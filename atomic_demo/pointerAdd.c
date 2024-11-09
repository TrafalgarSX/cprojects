#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main(void){
  char* test = "Hello, World!!!";
  uint8_t byte[10];
  uint8_t byte2[10];

  printf("test: %p\n", test);

  printf("byte: %p\n", byte);
  printf("byte: %p\n", byte + 1);
  printf("byte2: %p\n", byte2);

  for (int i = 0; i < 1; i++) {
    uint32_t* test_int_addr = (uint32_t*)malloc(1);
    uint8_t* test_char_addr = (uint8_t*)malloc(1);
    printf("test_int_addr: %p\n", test_int_addr);
    printf("test_char_addr: %p\n", test_char_addr);

    if((uint64_t)test_int_addr %  32 != 0x00){
        printf("test_int_addr is not aligned\n");
    }

    if((uint64_t)test_char_addr %  32 != 0x00){
        printf("test_char_addr is not aligned\n");
    }

    free(test_int_addr);
    free(test_char_addr);
  }

// test conclusion: void has unknown size, so it can't be used to calculate address
#if 0
  printf("%s\n", test + 5);
  printf("%s\n", (void *)test + 5);

  int test_int[0x10] = {0, 1, 3, 4, 5};
  void* test_void = &test_int[0x00];
  printf("%d\n", *(int *)(test_void + 0x04));
  printf("%d\n", *(test_int + 0x02));
#endif
  return 0x00;
}

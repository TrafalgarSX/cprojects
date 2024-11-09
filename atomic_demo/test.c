#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct A{
    int    a;
    char   b;
    short  c;
};
struct B{
    char   b;
    int    a;
    short  c;
};
#pragma pack(pop)

void TestFree(){
  char *test;
  if( test ==  NULL ){
    printf("main test is NULL\n");
  }
  printf("test is %p\n", test);
  free(test);
}

int main()
{
  //printf("%d\n", sizeof(struct A));
  //printf("%d\n", sizeof(struct B));

  char *ptr = (char *)malloc(10);
  free(ptr);
  printf("test first free end\n");

  TestFree();
  
  printf("test end\n");
  return 0;
}

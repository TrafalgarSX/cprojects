#include <stdio.h>


int main()
{

  const char c = 'c';
  char* pc;

  //c = 'a';  // not allowed
  const char** pcc = &pc;
  *pcc = &c;
  *pc = 'B';

  printf("c = %c\n", c);
  
  return 0;
}

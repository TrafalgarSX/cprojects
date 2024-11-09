#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void * HANDLE;
typedef unsigned char u8;
typedef unsigned int u32;

#define IF_FAIL_GOTO(x)                                                        \
  if (x) {                                                                     \
    goto END;                                                                  \
  }
#define IN  
#define OUT
#define INOUT

u32 ptpFunc(IN const u32 u32Type, OUT char **ppszData, OUT u32 *pu32DataLen) {
  u32 u32Result = 0;
  u32 u32InnerDataLen;
  char *pcData = NULL;

  if (NULL == pu32DataLen) {
    u32Result = -1;
    goto END;
  }

  switch (u32Type) {
  case 1: {
    pcData = "Hello World";
    u32InnerDataLen = strlen(pcData);   // 0x00 
    break;
  };
  case 2: {
    pcData = "Another Hello World";
    u32InnerDataLen = strlen(pcData);
    break;
  };
  default: {
    u32Result = -1;
    goto END;
  };
  };

  if (NULL == ppszData) {
    *pu32DataLen = u32InnerDataLen;
    goto END;
  }
  memcpy(*ppszData, pcData, u32InnerDataLen);

END:
  return u32Result;
}

int pFunc(IN const u32 u32DataLen, OUT u8 *pu8Data) {

  u32 u32Result = 0;
  char *pcData = "Hello World";
  u32 u32InnerDataLen = strlen(pcData);

  if (NULL == pu8Data) {
    u32Result = -1;
    goto END;
  }

  if (u32DataLen < u32InnerDataLen) {
    u32Result = -1;
    goto END;
  }

  memcpy(pu8Data, pcData, u32InnerDataLen);

END:
  return u32Result;
}

void test_sizeof(char* name, char* address, char another_address[]){
  printf("parameter:sizeof(name) = %d\n", sizeof(name));
  printf("parameter:sizeof(address) = %d\n", sizeof(address));
  printf("parameter:sizeof(another_address) = %d\n", sizeof(another_address));
}

int main(void) {
  // u32 u32Result = 0;
  // char *pcData = NULL;
  // u32 u32DataLen = 0;
  // u8 pu8Data[0x20] = {0x00};
  // char* pszName;

  // pszName = NULL;



  // // pointer to pointer function demonstration
  // u32Result = ptpFunc(1, NULL, &u32DataLen);
  // IF_FAIL_GOTO(0x00 != u32Result);
  // pcData = (char *)malloc(u32DataLen + 1);
  // memset(pcData, 0x00, u32DataLen + 1);
  // u32Result = ptpFunc(1, &pcData, &u32DataLen);
  // IF_FAIL_GOTO(0x00 != u32Result);

  // // pcData[u32DataLen] = '\0';
  // printf("%s\n", pcData);

  // // pointer function demonstration
  // u32Result = pFunc(sizeof(pu8Data), pu8Data);
  // printf("%s\n", (char *)pu8Data);

// END:
  // printf("u32Result: %d\n", u32Result);
  // return u32Result;
  printf("int bytes %d\n", sizeof(int));
  printf("unsigned int bytes %d\n", sizeof(unsigned int));
  printf("long bytes %d\n", sizeof(long));
  printf("long long bytes %d\n", sizeof(long long));
  printf("pointer bytes %d\n", sizeof((void*)NULL));

  char* name = (char*)malloc(10);
  char address[10] = "wuhan";
  printf("argument name sizeof %d\n", sizeof(name));
  printf("argument address sizeof %d\n", sizeof(address));

  test_sizeof(name, address, address);

  return 0x00;
}

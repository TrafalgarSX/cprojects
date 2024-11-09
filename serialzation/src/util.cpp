#include <CoSign.h>
#include <iostream>

u32 Hex2Asc(const void *pvHex, u32 u32HexLen, char *pszAsc) {
  u32 u32Idx;
  u8 *pu8Hex = (u8 *)pvHex;
  for (u32Idx = 0; u32Idx < u32HexLen; u32Idx++) {
    sprintf(pszAsc + u32Idx * 2, "%02X", pu8Hex[u32Idx]);
  }
  return u32Idx * 2;
}

u32 Asc2Hex(const char *pszAsc, u32 u32AscLen, void *pvHex) {
  u32 u32Idx;
  u8 *pu8Hex = (u8 *)pvHex;
  for (u32Idx = 0; u32Idx < u32AscLen; u32Idx++) {
    sscanf(pszAsc + u32Idx * 2, "%02X", &pu8Hex[u32Idx]);
  }
  return u32Idx;
}


/*
 *  这里并不需要重新分配内存，只需要将原来的内存进行重新组装即可 
 *  flatbuffers可以自动使用序列化大小的内存，所以这里不需要重新分配内存
  u8 *pu8CoSign = NULL;
  u8 *pu8CoSignJava = NULL;
  // CoSign对象的序列化缓存
  pu8CoSign = (u8 *)malloc(u32CoSignLen);
  // CoSignJava对象的序列化缓存
  pu8CoSignJava = (u8 *)malloc(u32DataLen - 4 - u32CoSignLen);

  memcpy(pu8CoSign, pu8Data + 4, u32CoSignLen);
  memcpy(pu8CoSignJava, pu8Data + 4 + u32CoSignLen,
         u32DataLen - 4 - u32CoSignLen);

  // 反序列化操作
  auto pCoSignFlat = server::GetBlkCoSign(pu8CoSign);
  auto pCoSignJava = javaserver::GetBlkCoSignJava(pu8CoSignJava);

  free(pu8CoSign);
  free(pu8CoSignJava);

 */


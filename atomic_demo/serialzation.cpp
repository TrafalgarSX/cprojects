#include <iostream>

typedef long ETBOOL;
typedef void *ETHANDLE;
typedef unsigned int u32;
typedef unsigned char u8;

typedef struct BlkCoSignJavaTag {
  ETBOOL m_bServer;
  ETHANDLE m_hCoSign;
  ETBOOL m_bFinal;
} BlkCoSignJava;

typedef struct BlkCoSignTag {
  u32 m_u32Version;
  u8 m_pu8PriKey[0x20];
  u8 m_pu8HashValue[0x20];
  u8 m_pu8ValueK[0x20];
  u8 m_pu8PointQ[0x40];
  u8 m_pu8R[0x20];
  u32 m_32Num;
} BlkCoSign;

u32 Serialization(const BlkCoSignJava *pCoSignJava, u8 **ppu8Data,
                  u32 *pu32DataLen) {
  u32 result;

  return result;
}

u32 DeSerialization(const u8 *pu8Data, u32 u32DataLen,
                    BlkCoSignJava **ppCoSignJava) {
  u32 result;

  return result;
}

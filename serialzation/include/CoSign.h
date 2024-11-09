#ifndef _COSIGN_H_ 

#define _COSIGN_H_

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
  u32 m_u32Num;
} BlkCoSign;


#	define ETCOSIGNTEST_DEFAULT_KEYS		"\xE8\x9E\xE8\x59\xD9\x5C\xCB\x96\x19\x72\x29\xAF\xD9\xC3\x71\x69\xE8\x9E\xE8\x59\xD9\x5C\xCB\x96\x19\x72\x29\xAF\xD9\xC3\x71\x69"
#	define ETCOSIGNTEST_DEFAULT_KEY		"\xE8\x9E\xE8\x59\xD9\x5C\xCB\x96\x19\x72\x29\xAF\xD9\xC3\x71\x69"
#	define ETCOSIGNTEST_ROOTKEY_BOX		"\x7A\xD1\x0E\x43\x63\x02\xEA\xEA\x49\x86\x68\x67\x3F\x8F\x1F\x6F"
#	define ROOTKEY_DEVICE				"\x7A\xD1\x0E\x43\x63\x02\xEA\xEA\x49\x86\x68\x67\x3F\x8F\x1F\x6F"


u32 Serialization(const BlkCoSignJava *pCoSignJava, const BlkCoSign *pCoSign,
                  u8 **ppu8Data, u32 *pu32DataLen);
u32 DeSerialization(const u8 *pu8Data, u32 u32DataLen,
                    BlkCoSignJava **ppCoSignJava);


u32 Hex2Asc(const void *pvHex, u32 u32HexLen, char* pszAsc);
u32 Asc2Hex(const char *pszAsc, u32 u32AscLen, void *pvHex);



#endif // _COSIGN_H_

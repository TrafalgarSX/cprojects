#ifndef _SM9_H_
#define _SM9_H_

typedef unsigned int       uint32_t;
typedef unsigned char      uint8_t;

#define INOUT
#define IN
#define OUT

#ifdef __cplusplus
extern "C" 
{
#endif

uint32_t SM9_Init();

uint32_t SM9_Release();

uint32_t SM9_GenMasterSignKey(INOUT uint8_t* pu8Ks, OUT uint8_t *pu8Ppubs);
uint32_t SM9_GenMasterEncKey(INOUT uint8_t* pu8Ke, OUT uint8_t* pu8Ppube);

uint32_t SM9_GenerateSignKey(IN const uint8_t* pu8Ks, IN const uint8_t* pu8ID, IN uint32_t u32IDLen, OUT uint8_t* pu8dsA);
uint32_t SM9_GenerateEncryptKey(IN const uint8_t* pu8Ke, IN const uint8_t* pu8ID, IN uint32_t u32IDLen , OUT uint8_t* pu8deB);

uint32_t SM9_Sign(IN const uint8_t* pu8Ppubs, IN const uint8_t* pu8dsA, 
		IN const uint8_t* pu8Msg, IN uint32_t u32MsgLen, 
		OUT uint8_t* pu8H, OUT uint8_t* pu8S);

uint32_t SM9_Verify(IN const uint8_t* pu8Ppubs, IN const uint8_t* pu8IDA, IN uint32_t u32IDALen,
		IN const uint8_t* pu8Msg, IN uint32_t u32MsgLen, 
		IN const uint8_t* pu8H, IN const uint8_t* pu8S);

uint32_t SM9_Encrypt(IN const uint8_t* pu8Ppube, 
		IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen,
		IN const uint8_t* pu8Msg, IN uint32_t u32MsgLen, 
		OUT uint8_t* pu8C, OUT uint32_t *pu32CLen);

uint32_t SM9_Decrypt(IN const uint8_t* pu8deB, IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen,
		IN const uint8_t* pu8C, IN uint32_t u32CLen, 
		OUT uint8_t* pu8M, OUT uint32_t *pu32MLen);

uint32_t SM9_Key_encap(IN const uint8_t* pu8Ppube, 
		IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen, IN uint32_t u32KLen, 
		OUT uint8_t* pu8C, OUT uint8_t* pu8K);

uint32_t SM9_Key_decap(IN const uint8_t* pu8IDB, IN uint32_t u32IDBLen, IN const uint8_t* pu8deB, IN const uint8_t* pu8C, IN uint32_t u32KLen, OUT uint8_t* pu8K);

uint32_t test(char* tarray);

#ifdef __cplusplus
};
#endif

#endif // _SM9_H_

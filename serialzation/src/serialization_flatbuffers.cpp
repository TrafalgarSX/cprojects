#include <CoSign.h>
#include <cstring>
#include <iostream>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef USE_FLATBUFFERS
#  include <blkcosign_generated.h>
#  include <blkcosignjava_generated.h>

/*
*  ppu8Data在函数内部申请内存，需要在外部释放, 申请内存大小为*pu32DataLen
*/
u32 Serialization(const BlkCoSignJava *pCoSignJava, const BlkCoSign *pCoSign,
                  u8 **ppu8Data, u32 *pu32DataLen) {
  u32 result = 0;
  flatbuffers::FlatBufferBuilder builder;
  flatbuffers::FlatBufferBuilder builderJava;
  u32 u32CoSignLen;
  // 序列化后的缓存, 由FlatBufferBuilder申请内存,并在函数内部释放
  u8 *buf, *bufJava;

  // 这里是否需要判断pCoSignJava和pCoSign是否为空
  // if (pCoSignJava == NULL || pCoSign == NULL) {
  //   return 1;
  // }

  // BlkCoSign序列化
  auto coSignOffset = server::CreateBlkCoSign(
      builder, pCoSign->m_u32Version,
      builder.CreateVector(pCoSign->m_pu8PriKey, 0x20),
      builder.CreateVector(pCoSign->m_pu8HashValue, 0x20),
      builder.CreateVector(pCoSign->m_pu8ValueK, 0x20),
      builder.CreateVector(pCoSign->m_pu8PointQ, 0x40),
      builder.CreateVector(pCoSign->m_pu8R, 0x20), pCoSign->m_u32Num);
  // builder.Finish(coSignOffset);
  builder.FinishSizePrefixed(coSignOffset);
  buf = builder.GetBufferPointer();
  // BlkCoSign序列化后的长度
  *pu32DataLen = builder.GetSize();
  u32CoSignLen = *pu32DataLen;

  // BlkCoSignJava序列化
  auto coSignJavaOffset = javaserver::CreateBlkCoSignJava(
      builderJava, pCoSignJava->m_bServer, (uint64_t)pCoSignJava->m_hCoSign,
      pCoSignJava->m_bFinal);
  builderJava.Finish(coSignJavaOffset);
  bufJava = builderJava.GetBufferPointer();

  // BlkCoSignJava序列化后的长度和BlkCoSign序列化后的长度相加
  *pu32DataLen += builderJava.GetSize();
  // 分配内存, 开头4个字节存放BlkCoSign序列化后的长度
  *ppu8Data = (u8 *)malloc(*pu32DataLen);

  memcpy(*ppu8Data, buf, u32CoSignLen);
  std::cout << "builder.GetSize() = " << u32CoSignLen << std::endl;

  memcpy(*ppu8Data + u32CoSignLen, bufJava, builderJava.GetSize());

END:
  return result;
}

/*
 * 调用该函数要注意, 传入的ppCoSignJava内部会分配内存, 用完后需要调用
 * FreeCoSignJava函数释放内存
 *
 * 这里的pu8Data是BlkCoSign序列化后的数据, 前4个字节存放的是BlkCoSign序列化后的长度
*/
u32 DeSerialization(const u8 *pu8Data, u32 u32DataLen,
                    BlkCoSignJava **ppCoSignJava) {
  u32 result = 0;
  u32 u32CoSignLen = 0;
  BlkCoSign *pCoSign = NULL;
  
  // TODO 这里是否需要判断pu8Data是否为空?
  // if (pu8Data == NULL || u32DataLen < 4) {
  //   result = 1;
  //   return result;
  // }

  u32CoSignLen = flatbuffers::ReadScalar<flatbuffers::uoffset_t>(pu8Data); //得到真实数据长度
  std::cout << "u32CoSignLen = " << u32CoSignLen << std::endl;

  flatbuffers::Verifier verifer(pu8Data, u32CoSignLen + 4);
  if( !server::VerifySizePrefixedBlkCoSignBuffer(verifer)){
    std::cout << "VerifyBlkCoSignBuffer failed" << std::endl;
    return result;
  }

  // 反序列化
  auto pCoSignFlat = server::GetSizePrefixedBlkCoSign(pu8Data);
  // auto pCoSignFlat = server::GetBlkCoSign(pu8Data + 4);
  auto pCoSignJava = javaserver::GetBlkCoSignJava(pu8Data + 4 + u32CoSignLen);

  // 将FlatBuffer内部的数据拷贝到CoSign对象中
  pCoSign = (BlkCoSign *)malloc(sizeof(BlkCoSign));
  auto pCoSignPriKey = pCoSignFlat->m_pu8PriKey();
  auto pCoSignHash = pCoSignFlat->m_pu8HashValue();
  auto pCoSignValueK = pCoSignFlat->m_pu8ValueK();
  auto pCoSignR = pCoSignFlat->m_pu8R();
  auto pCoSignPointQ = pCoSignFlat->m_pu8PointQ();

  for (int i = 0; i < 0x20; i++) {
    pCoSign->m_pu8PriKey[i] = pCoSignPriKey->Get(i);
    pCoSign->m_pu8HashValue[i] = pCoSignHash->Get(i);
    pCoSign->m_pu8ValueK[i] = pCoSignValueK->Get(i);
    pCoSign->m_pu8R[i] = pCoSignR->Get(i);
    pCoSign->m_pu8PointQ[i] = pCoSignPointQ->Get(i);
  }
  for (int i = 0x20; i < 0x40; i++) {
    pCoSign->m_pu8PointQ[i] = pCoSignPointQ->Get(i);
  }
  pCoSign->m_u32Version = pCoSignFlat->m_u32Version();
  pCoSign->m_u32Num = pCoSignFlat->m_u32Num();

  // 组装最后的数据
  *ppCoSignJava = (BlkCoSignJava *)malloc(sizeof(BlkCoSignJava));
  (*ppCoSignJava)->m_bServer = pCoSignJava->m_bServer();
  (*ppCoSignJava)->m_bFinal = pCoSignJava->m_bFinal();
  (*ppCoSignJava)->m_hCoSign = pCoSign;
 
  result = 0;
  return result;
}

#endif

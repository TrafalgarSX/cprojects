#include <CoSign.h>
#include <cstring>
#include <iostream>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

#if USE_PROTOBUF
#  include <BlkCoSignJava.pb.h>

u32 Serialization(const BlkCoSignJava *pCoSignJava, const BlkCoSign *pCoSign,
                  u8 **ppu8Data, u32 *pu32DataLen) {
  u32 result = 0;

  proto::BlkCoSignJava blkCoSignJava;
  proto::BlkCoSign blkCoSign;

  blkCoSignJava.set_m_bserver(pCoSignJava->m_bServer);
  blkCoSignJava.set_m_bfinal(pCoSignJava->m_bFinal);

  blkCoSign.set_m_u32version(pCoSign->m_u32Version);
  blkCoSign.set_m_u32num(pCoSign->m_u32Num);
  blkCoSign.set_m_pu8prikey(pCoSign->m_pu8PriKey, 0x20);
  blkCoSign.set_m_pu8hashvalue(pCoSign->m_pu8HashValue, 0x20);
  blkCoSign.set_m_pu8valuek(pCoSign->m_pu8ValueK, 0x20);
  blkCoSign.set_m_pu8pointq(pCoSign->m_pu8PointQ, 0x40);
  blkCoSign.set_m_pu8r(pCoSign->m_pu8R, 0x20);

  // 要求在堆上分配内存, 由protobuf自动释放
  // 如果不是在堆上分配内存, 会导致程序崩溃
  // blkCoSignJava.set_allocated_m_pcosign(&blkCoSign);
  blkCoSignJava.mutable_m_pcosign()->CopyFrom(blkCoSign);

  *pu32DataLen =  blkCoSignJava.ByteSize();
  *ppu8Data = (u8 *)malloc(*pu32DataLen);
  memset(*ppu8Data, 0, *pu32DataLen);

  blkCoSignJava.SerializeToArray(*ppu8Data, *pu32DataLen);

  return result;
}


u32 DeSerialization(const u8 *pu8Data, u32 u32DataLen,
                    BlkCoSignJava **ppCoSignJava) {
  u32 result = 0;
  proto::BlkCoSignJava blkCoSignJava;
  BlkCoSign *pCoSign = (BlkCoSign *)malloc(sizeof(BlkCoSign));
  BlkCoSignJava *pCoSignJava = (BlkCoSignJava *)malloc(sizeof(BlkCoSignJava));

  blkCoSignJava.ParseFromArray(pu8Data, u32DataLen);

  pCoSign->m_u32Version = blkCoSignJava.m_pcosign().m_u32version();
  pCoSign->m_u32Num = blkCoSignJava.m_pcosign().m_u32num();
  memcpy(pCoSign->m_pu8PriKey, blkCoSignJava.m_pcosign().m_pu8prikey().c_str(), 0x20);
  memcpy(pCoSign->m_pu8HashValue, blkCoSignJava.m_pcosign().m_pu8hashvalue().c_str(), 0x20);
  memcpy(pCoSign->m_pu8ValueK, blkCoSignJava.m_pcosign().m_pu8valuek().c_str(), 0x20);
  memcpy(pCoSign->m_pu8PointQ, blkCoSignJava.m_pcosign().m_pu8pointq().c_str(), 0x40);
  memcpy(pCoSign->m_pu8R, blkCoSignJava.m_pcosign().m_pu8r().c_str(), 0x20);

  pCoSignJava->m_bServer = blkCoSignJava.m_bserver();
  pCoSignJava->m_bFinal = blkCoSignJava.m_bfinal();
  pCoSignJava->m_hCoSign = pCoSign;

  *ppCoSignJava = pCoSignJava;

  return result;
}

#endif

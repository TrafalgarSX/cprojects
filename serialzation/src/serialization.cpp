#include <CoSign.h>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  // 计时开始
  clock_t start, end;
  start = clock();

  u8 *pu8Data = NULL;
  u32 u32DataLen = 0;
  char *pszSerialzationAsc;
  char pszPriKeyAsc[0x41] = {0};
  char pszHashAsc[0x41] = {0};
  char pszValueKAsc[0x41] = {0};
  char pszPointQAsc[0x81] = {0};
  char pszRAsc[0x41] = {0};
  BlkCoSignJava *pCoSignJavaDeSerial = NULL;
  BlkCoSignJava *pCoSignJava = NULL;
  BlkCoSign *pCoSign = NULL;

  // 先拼接测试数据
  pCoSignJava = (BlkCoSignJava *)malloc(sizeof(BlkCoSignJava));
  pCoSignJava->m_bServer = 0x1234;
  pCoSignJava->m_hCoSign = (ETHANDLE)0x12345678;
  pCoSignJava->m_bFinal = 0x4321;
  // 成员结构体测试数据
  pCoSign = (BlkCoSign *)malloc(sizeof(BlkCoSign));
  memset(pCoSign, 0x00, sizeof(BlkCoSign));
  pCoSign->m_u32Version = 0x9876543;
  pCoSign->m_u32Num = 0x12345678;
  memcpy(pCoSign->m_pu8PriKey, (u8 *)ETCOSIGNTEST_DEFAULT_KEYS, 0x20);
  memcpy(pCoSign->m_pu8HashValue, (u8 *)ETCOSIGNTEST_ROOTKEY_BOX, 0x10);
  memcpy(pCoSign->m_pu8ValueK, (u8 *)ETCOSIGNTEST_DEFAULT_KEY, 0x10);
  memcpy(pCoSign->m_pu8PointQ, (u8 *)ETCOSIGNTEST_ROOTKEY_BOX, 0x10);
  memcpy(pCoSign->m_pu8R, (u8 *)ROOTKEY_DEVICE, 0x10);

//   memset(pCoSign->m_pu8PriKey, 0x11, 0x20);
//   memset(pCoSign->m_pu8HashValue, 0x22, 0x20);
//   memset(pCoSign->m_pu8ValueK, 0x33, 0x20);
//   memset(pCoSign->m_pu8PointQ, 0x44, 0x40);
//   memset(pCoSign->m_pu8R, 0x55, 0x20);
  
  // 使用测试数据进行序列化
  Serialization(pCoSignJava, pCoSign, &pu8Data, &u32DataLen);
  pszSerialzationAsc = (char *)malloc(u32DataLen * 2 + 1);
  Hex2Asc(pu8Data, u32DataLen, pszSerialzationAsc);
  // 实际使用中，这里记得释放pu8Data
  // pszSerialzationAsc在返回给Jni层后，Jni层记得释放

  std::cout << "pu8Data to Asc: " << pszSerialzationAsc << std::endl;
  std::cout << "Serialization DataLen: " << u32DataLen << std::endl;

  // 反序列化
  DeSerialization(pu8Data, u32DataLen, &pCoSignJavaDeSerial);
  // 实际使用中，这里记得释放pu8Data, 真实使用中这里的pu8Data是Asc2Hex后的数据
  // pCoSignJavaDeSerial在返回给Jni层后，并不能释放， 后面还要用
  // checkfinal的时候再释放

  // 计算时间
  end = clock();
  std::cout << "----------------" << std::endl;
  std::cout << "DeSerialization Time: "
            << ((double)(end - start) / CLOCKS_PER_SEC) * 1000 << "ms"
            << std::endl;
  std::cout << "----------------" << std::endl;

  // 打印反序列化结果
  std::cout << "pCoSignJavaDeSerial->m_bServer: " << std::setbase(16) << pCoSignJavaDeSerial->m_bServer
            << std::endl;
  std::cout << "pCoSignJavaDeSerial->m_bFinal: " << std::setbase(16) << pCoSignJavaDeSerial->m_bFinal
            << std::endl;

  BlkCoSign *pTmpCoSign = (BlkCoSign *)(pCoSignJavaDeSerial->m_hCoSign);
  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_u32Version: "
            << std::setbase(16) << pTmpCoSign->m_u32Version << std::endl;
  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_32Num: " << std::setbase(16) << pTmpCoSign->m_u32Num
            << std::endl;
  Hex2Asc(pTmpCoSign->m_pu8PriKey, 0x20, pszPriKeyAsc);
  Hex2Asc(pTmpCoSign->m_pu8HashValue, 0x20, pszHashAsc);
  Hex2Asc(pTmpCoSign->m_pu8ValueK, 0x20, pszValueKAsc);
  Hex2Asc(pTmpCoSign->m_pu8R, 0x20, pszRAsc);
  Hex2Asc(pTmpCoSign->m_pu8PointQ, 0x40, pszPointQAsc);

  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_pu8PriKey: " << pszPriKeyAsc
            << std::endl;
  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_pu8HashValue: " << pszHashAsc
            << std::endl;
  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_pu8ValueK: " << pszValueKAsc
            << std::endl;
  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_pu8R: " << pszRAsc << std::endl;
  std::cout << "pCoSignJavaDeSerial->m_hCoSign->m_pu8PointQ: " << pszPointQAsc
            << std::endl;

  return 0;
}

#include <bitset>
#include <iostream>

void print_binary(int value) {
  for (int i = sizeof(int) * CHAR_BIT - 1; i >= 0; i--) {
    if (value & (1 << i)) {
      std::cout << "1";
    } else {
      std::cout << "0";
    }
  }
  std::cout << std::endl;
}

// 这里用unsigned是因为这样 >> 1 时最高位会补0, 而不是补1
void bin(unsigned value) {
  if (value > 1) {
    bin(value >> 1);
  }

  printf("%d", value & 1);
}

void printunsigned(unsigned value) {
  printf("unsigned: %u\n", value);
  printf("int: %d\n", INT_MAX);
  printf("unsigned int: %u\n", UINT_MAX);

  int number = value;
  printf("int: %d\n", number);
}

// 机器中确实用补码来表示负数
int main(void) {
  int number = -10;
  std::string binary;

  printunsigned(number);

  // C语言通用方法
  // 用位运算来转换10进制到2进制(输出字符串)
  print_binary(number);

  // 方法二 用递归来转换10进制到2进制(输出字符串)
  bin(number);
  printf("\n");

  // 用bitset来转换10进制到2进制(输出字符串)
  binary = std::bitset<8>(-number).to_string();
  std::cout << binary << std::endl;

  binary = std::bitset<8>(number).to_string();
  std::cout << binary << std::endl;

  binary = std::bitset<8>(number & 1).to_string();
  std::cout << binary << std::endl;

  binary = std::bitset<8>(number | 1).to_string();
  std::cout << binary << std::endl;

  // c++14开始支持二进制字面量
  int binary_number = 0b101010;
  unsigned long decimal = std::bitset<8>(binary).to_ulong();
  std::cout << decimal << "\n";
  return 0;
}

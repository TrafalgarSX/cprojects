#include <iostream>
#include <regex>
#include <string>

int main() {
  std::string str =
      "http://testlksjdfljk:8092/httpdlfkjlkj?r=123456789&s=987654431";
  std::string pattern{"s=[0-9]+"};
  // std::string pattern{"http"};
  std::regex reg(pattern, std::regex_constants::ECMAScript);
  std::smatch match;
  bool ret;
  
  // 要求字符串完全匹配正则表达式
  ret = std::regex_match(str, reg);
  if (ret) {
    std::cout << "match" << std::endl;
  } else {
    std::cout << "not match" << std::endl;
  }
  
  // 获得第一个匹配的字符串
  ret = std::regex_search(str, match, reg);
  if (ret) {
    for (auto m: match) {
       std::cout << m << std::endl;
    }
    // for (int i = 0; i < match.size(); ++i) {
    //   std::cout << match[i] << std::endl;
    // }
  } else {
    std::cout << "not match" << std::endl;
  }
  return 0;
}

#include <iostream>
#include <sstream>

using namespace std;

int main() {
    char str[] = "He\0llo\0world\0!";   // 包含两个'\0'的字符串
    std::string s(str, sizeof(str));
    std::cout << s << std::endl;
    return 0;
}

// #include <sstream>
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

// c99开始支持 VLA i.e.变长数组
// cpp也支持 哪个版本开始不清楚
// void vla_func(void) {
//   int n;
//   scanf("%d", &n);

//   int array[n];
//   /* 错误的，使用变长数组不能进行初始化
//    * 因为长度在运行时才能确定
//    * int array[n] = {0};
//    *
//    */
//   for (int i = 0; i < n; i++) {
//     printf("%d\t", array[i]);
//   }
// }

int main() {
  // vla_func();
  //
  char *str = "Hello\0world\0!"; // 包含两个'\0'的字符串
  char number[21] = {0};
  unsigned int pu32Header[3] = {0};

  cout << sizeof(pu32Header) << endl;
  cout << "cpp doesn't support vla" << endl;

  char *test = new char[]{0x31, 0x32, 0x33, 0x00};
  cout << test << endl;

  // print to file
  FILE *fp = fopen("test.txt", "w");
  fprintf(fp, "%s", test);
  fclose(fp);

  // print to stdout
  fprintf(stdout, "%s", test);


  // sprintf(number, "%lld", LLONG_MAX);
  // printf("%s: length is :%d\n", number, strlen(number));
  // stringstream ss(str);           // 创建字符串流
  // string s;
  // while (getline(ss, s, '\0')) { // 以'\0'为分隔符读取
  // cout << s << endl;           // 输出读取的字符串
  // }
  return 0;
}

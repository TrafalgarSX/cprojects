#include <stdio.h>
#include <time.h>

int main() {
  time_t t;
  struct tm *tmp;
  char timestamp[20];

  t = time(NULL);
  tmp = localtime(&t);

  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tmp);
  printf("当前时间戳为：%s\n", timestamp);

  return 0;
}

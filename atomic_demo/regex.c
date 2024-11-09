#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool reg_matches(const char *str, const char *pattern) {
  regex_t re;
  int ret;
  regmatch_t pmatch;

  if (regcomp(&re, pattern, REG_EXTENDED) != 0)
    return false;

  ret = regexec(&re, str, 1, &pmatch, 0);
  regfree(&re);

  if (ret == 0) {
    printf("%c, %d\n", str[pmatch.rm_so], pmatch.rm_so);
    printf("%c, %d\n", str[pmatch.rm_eo], pmatch.rm_eo);
    for (size_t i = pmatch.rm_so; i < pmatch.rm_eo; i++) {
      printf("%c", str[i]);
    }
    printf("\n");
    return true;
  }

  return false;
}

/* windows下找个能用的regex库太难了。这个是和linux下一样的。
 * 网上教人下载的都没有用，不兼容。 直接用Qt opt文件夹下面
 * 的regex库就可以了。
 */
int main(void) {
  static const char *pattern = "r=[0-9]+";
  const char *test =
      "http://testlksjdfljk:8092/lksjdlfkjlkj?r=123456789&s=987654431";
  const char *find = NULL;

  // if(reg_matches("http://testlksjdfljk:8092/lksjdlfkjlkj?r=12345678&s=987654431",
  // pattern))
  if (reg_matches(test, pattern))
    printf("true\n");
  else
    printf("false\n");

  find = strstr(test, "r=");
  if (find) {
    printf("%.*s\n", 11, find);
    printf("%.*s\n", 9, find + 2);
  }

  return 0;
}

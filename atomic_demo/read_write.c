#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read() {
  char *err_buf = NULL;
  char buf[1024] = {0};
  FILE *fp = NULL;

  errno = fopen_s(&fp, "input.txt", "r");
  if (0x00 != errno) {
    /* size_t errmsglen = strerrorlen_s(errno) + 1; */
    size_t errmsglen = 100;
    err_buf = calloc(1, errmsglen);
    strerror_s(err_buf, errmsglen, errno);
    printf("%s\n", err_buf);
  }

  while (fgets(buf, 1024, fp) != NULL) {
    printf("%s\n", buf);
  }

  fclose(fp);
  return 0;
}

int write() {
  char *err_buf = NULL;
  FILE *fp = NULL;

  errno = fopen_s(&fp, "input.txt", "w");
  if (0x00 != errno) {
    /* size_t errmsglen = strerrorlen_s(errno) + 1; */
    size_t errmsglen = 100;
    err_buf = calloc(1, errmsglen);
    strerror_s(err_buf, errmsglen, errno);
    printf("%s\n", err_buf);
  }

  for (int i = 0; i < 1024; ++i) {
    fprintf(fp, "%x\t", i);
  }

  fclose(fp);
  return 0;
}

int main(int argc, char *argv[]) {
  write();
  read();
  return 0;
}

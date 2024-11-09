#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *source_file, *target_file;
  char source_path[100], target_path[100], buffer[1024];
  int read_size;

  printf("请输入源文件路径: ");
  scanf("%s", source_path);
  printf("请输入目标文件路径: ");
  scanf("%s", target_path);

  source_file = fopen(source_path, "rb");
  if (source_file == NULL) {
    printf("无法打开源文件!\n");
    return 1;
  }

  target_file = fopen(target_path, "wb");
  if (target_file == NULL) {
    printf("无法创建目标文件!\n");
    fclose(source_file);
    return 1;
  }

  while ((read_size = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
    fwrite(buffer, 1, read_size, target_file);
  }

  printf("备份文件成功!\n");

  fclose(source_file);
  fclose(target_file);

  return 0;
}

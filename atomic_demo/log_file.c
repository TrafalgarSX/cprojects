#include <stdio.h>

int main(void)
{
    FILE *fp;
    char str[100];
    int i;

    fp = fopen("log.txt", "w");
    for (i = 0; i < 10; i++) {
        fprintf(fp, "This is line 0x%x \n", 0x100 + i);
    }
    fclose(fp);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// windows下没有getopt函数，linux下在头文件unistd.h中
int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "if:lr")) != -1) {
        switch (opt) {
        case 'i':
        case 'l':
        case 'r':
            printf("option: %c\n", opt);
            break;
        case 'f':
            printf("filename: %s\n", optarg);
            break;
        case ':':
            printf("option needs a value\n");
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;
        }
    }
    for (; optind < argc; optind++)
        printf("argument: %s\n", argv[optind]);
    return 0;
}

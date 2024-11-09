#include <stdio.h>

int main(){
    // if cpp compiler
#if __cplusplus
    printf("C++\n");
#else
    printf("C\n");
#endif
    int a;
    scanf("%d", &a);
    if( a == 1 ){
        goto label1;
    }
    int b = 10;
    // int b;
    printf("b = %d\n", b);

label1:
    return 0;
}


#include <stdio.h>
int main(){
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


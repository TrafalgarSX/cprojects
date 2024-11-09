#include<stdio.h>

int main(void){
#ifndef NDEBUG
	printf("release模式下有NDEBUG的宏");
#endif

}

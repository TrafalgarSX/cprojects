#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>

// clang time.c -o out/time.exe -DNOW
int main(void) {
	int	    year, month, day, hour, minute, sec;
	time_t  time_ptr;                            // a big integer
	struct  tm *tmp_ptr = NULL;

	//获取当前时间
	time(&time_ptr);
#ifdef NOW
	tmp_ptr = localtime(&time_ptr);
#else
	tmp_ptr = gmtime(&time_ptr);
#endif

	/* the difference with gmtime() and localtime() is :
	 * gmtime()转换出来的是 0时区的时间
	 * localtime()转换出来的是本地的时区
	 * 比如武汉现在的时区是 +8区，比UTC多8个多小时 
	 * localtime()转换出来的时间就比 gmtime()快 8个小时
	 * */

	year    = tmp_ptr->tm_year + 1900; // year表示从1900到现在过了多少年
	month   = tmp_ptr->tm_mon + 1;  //month 从0开始
	day     = tmp_ptr->tm_mday;

	hour    = tmp_ptr->tm_hour;
	minute  = tmp_ptr->tm_min;
	sec     = tmp_ptr->tm_sec;

	printf("%dyear%dmonth%dday:%dhour%dminute%dsec", year, month, day, hour, minute, sec);

	system("pause");
	return 0;
}

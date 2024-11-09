#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>

int main(void){
	int counter;
	double *ptr_d;
	FILE *ptr_fp;
	
	/* fopen fwrite/fread fclose */
	ptr_d = (double *)malloc(sizeof(double) * 10);
	if(!ptr_d)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}else
		printf("Memory allocation successful\n");

	for(counter = 0; counter < 10; counter++)
		ptr_d[counter] = (double) rand();

	if((ptr_fp = fopen("test.txt", "wb")) == NULL)
	{
		printf("File open failed\n");
		exit(1);
	}else
		printf("File open successful\n");

	if(fwrite(ptr_d, 10 * sizeof(double), 1, ptr_fp)!= 1)
	{
		printf("Write failed\n");
		exit(1);
	}else
		printf("Write successful\n");

	fclose(ptr_fp);
	free(ptr_d);

	ptr_d = (double*)malloc(sizeof(double) * 10);
	if(!ptr_d)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}else
		printf("Memory allocation successful\n");

	if((ptr_fp = fopen("test.txt", "rb")) == NULL)
	{
		printf("File open failed\n");
		exit(1);
	}else
		printf("File open successful\n");

	if(fread(ptr_d, 10 * sizeof(double), 1, ptr_fp)!= 1)
	{
		printf("Read failed\n");
		exit(1);
	}else
		printf("Read successful\n");

	fclose(ptr_fp);

	printf("The numbers read from file are:\n");
	for(counter = 0; counter < 10; counter++)
		printf("%f ", ptr_d[counter]);

	free(ptr_d);
	return 0;
}

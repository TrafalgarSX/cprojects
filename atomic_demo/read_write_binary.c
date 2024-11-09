#include <stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct rectTag{
	int x, y, w, h;
}rect;

int read(int count)
{
	char *err_buf = NULL;
	FILE *fp = NULL;
	rect my_record;

	errno = fopen_s(&fp, "input.txt", "rb");
	if (0x00 != errno)
	{
		/* size_t errmsglen = strerrorlen_s(errno) + 1; */
		size_t errmsglen = 100;
		err_buf = calloc(1, errmsglen);
		strerror_s(err_buf, errmsglen, errno);
		printf("%s\n", err_buf);
	}

	for (int i = 0; i < count; i++) {
		fread(&my_record, sizeof(rect), 1, fp);
		printf("%d\t%d\t%d\t%d\n", my_record.x, my_record.y, my_record.w, my_record.h);
	}

	fclose(fp);
	return 0;
}


int write()
{
	char *err_buf = NULL;
	FILE *fp = NULL;
	rect my_record;

	errno = fopen_s(&fp, "input.txt", "wb");
	if (0x00 != errno)
	{
		/* size_t errmsglen = strerrorlen_s(errno) + 1; */
		size_t errmsglen = 100;
		err_buf = calloc(1, errmsglen);
		strerror_s(err_buf, errmsglen, errno);
		printf("%s\n", err_buf);
	}

	for (int i = 0x00; i < 10; i++) {
		my_record.x = 10;
		my_record.y = 11;
		my_record.w = i;
		my_record.h = i;
		fwrite(&my_record, sizeof(rect), 1, fp);
	}

	fclose(fp);
	return 0;
}

int seek_write()
{
	char *err_buf = NULL;
	FILE *fp = NULL;
	rect my_record;

	errno = fopen_s(&fp, "input.txt", "wb");
	if (0x00 != errno)
	{
		/* size_t errmsglen = strerrorlen_s(errno) + 1; */
		size_t errmsglen = 100;
		err_buf = calloc(1, errmsglen);
		strerror_s(err_buf, errmsglen, errno);
		printf("%s\n", err_buf);
	}

	fseek(fp, sizeof(rect) * 10, SEEK_SET);

	my_record.x = 100;
	my_record.y = 111;
	my_record.w = 222;
	my_record.h = 333;
	fwrite(&my_record, sizeof(rect), 1, fp);

	fclose(fp);
	return 0;
}


int seek_read(int count)
{
	char *err_buf = NULL;
	FILE *fp = NULL;
	rect my_record;

	errno = fopen_s(&fp, "input.txt", "rb");
	if (0x00 != errno)
	{
		/* size_t errmsglen = strerrorlen_s(errno) + 1; */
		size_t errmsglen = 100;
		err_buf = calloc(1, errmsglen);
		strerror_s(err_buf, errmsglen, errno);
		printf("%s\n", err_buf);
	}

	fseek(fp, sizeof(rect) * count, SEEK_SET);

	fread(&my_record, sizeof(rect), 1, fp);
	printf("\n%d\t%d\t%d\t%d\n", my_record.x, my_record.y, my_record.w, my_record.h);

	fclose(fp);
	return 0;
}

int main(int argc, char **argv)
{
	write();

	read(10);

	/* seek_write(); */

	seek_read(8);

	return 0;
}

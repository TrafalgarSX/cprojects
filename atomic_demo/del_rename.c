#include <stdio.h>
#include<stdlib.h>
#include<string.h>

int remove_file(char *filename)
{
	if(remove(filename))
	{
		printf("%s Remove\n", filename);
	}else {
		printf("Remove %s Error\n", filename);
	}
	return 0;
}

int rename_file(char *org_filename, char *new_filename)
{
	if(rename(org_filename, new_filename) == 0)
	{
		printf("%s has been renamed %s\n", org_filename, new_filename);
	}else {
		printf("rename %s to %s Error\n", org_filename, new_filename);
	}

	return 0;
}

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Usage: input filename!");
		exit(1);
	}

	rename_file(argv[1], argv[2]);

	/* remove_file(argv[2]); */

	return 0;
}



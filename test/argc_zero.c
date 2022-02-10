#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc)
		printf("OK (argc != 0)\n");
	else
		fprintf(stderr, "FAIL (argc == 0)\n");

	return argc;
}

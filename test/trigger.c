#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	execve("/usr/bin/pkexec", NULL, NULL);
	fprintf(stderr, "%s\n", strerror(errno));
	return 0;
}

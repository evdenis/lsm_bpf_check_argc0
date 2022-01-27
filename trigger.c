#include <unistd.h>

int main(int argc, char *argv[])
{
	execve("/usr/bin/pkexec", NULL, NULL);
	return 0;
}

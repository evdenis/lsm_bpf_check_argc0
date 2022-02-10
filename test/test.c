#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>

int main(int argc, char *argv[])
{
	char self_path[PATH_MAX];
	char *const argp[] = {
		NULL
	};
	char *const envp[] = {
		NULL
	};

	readlink("/proc/self/exe", self_path, sizeof(self_path));
	strcpy(rindex(self_path, '/') + 1, "argc_zero");

	execve(self_path, argp, envp);
	fprintf(stderr, "%s\n", strerror(errno));

	return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[], char *envp[])
{
	if (!argc) {
		fprintf(stderr, "FAIL (argc == 0)\n");
		return 1;
	}

	if (*envp && !strcmp(*envp, "ARGC0_STOP")) {
		printf("OK (argc != 0)\n");
		return 0;
	}

	execve(argv[0], (char *const []){NULL},
			(char *const []){"ARGC0_STOP", NULL});
	fprintf(stderr, "OK (execve failed: %s)\n", strerror(errno));

	return 0;
}

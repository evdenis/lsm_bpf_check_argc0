#include "check_argc0_lsm.h"
#include <unistd.h>

static void check_fail(bool ok, const char *fmt, ...)
{
	va_list ap;

	if (ok)
		return;

	va_start(ap, fmt);
	fprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");

	exit(1);
}

int main(int argc, char *argv[])
{
	struct check_argc0_lsm *lsm;

	lsm = check_argc0_lsm__open_and_load();
	check_fail(lsm, "failed to load bpf");

	check_argc0_lsm__attach(lsm);

	for(;;) {
		sleep(1);
	}
	return 0;
}

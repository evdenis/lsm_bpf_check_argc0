#include "check_argc0_lsm.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
	check_argc0_lsm__attach(check_argc0_lsm__open_and_load());
	for(;;) {
		sleep(1);
	}
	return 0;
}

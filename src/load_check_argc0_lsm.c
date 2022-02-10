#include "check_argc0_lsm.h"
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
#include <time.h>
#include <errno.h>
#include "common.h"

#define check_fail_handle_goto(cond, err_stmt, label, fmt, ...) ({ \
	if (cond) {                                                \
		fprintf(stderr, fmt "\n", ## __VA_ARGS__);         \
		err_stmt;                                          \
		goto label;                                        \
	}                                                          \
})

#define check_fail_goto(cond, label, fmt, ...) ({            \
	if (cond) {                                          \
		fprintf(stderr, fmt "\n", ## __VA_ARGS__);   \
		goto label;                                  \
	}                                                    \
})

#define check_fail_exit(cond, fmt, ...) ({                   \
	if (cond) {                                          \
		fprintf(stderr, fmt "\n", ## __VA_ARGS__);   \
		exit(1);                                     \
	}                                                    \
})


#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME
#define check_fail(...) GET_MACRO(__VA_ARGS__,            \
				  check_fail_handle_goto, \
				  check_fail_goto,        \
				  check_fail_exit)(__VA_ARGS__)

int libbpf_print_fn(enum libbpf_print_level level,
		    const char *format, va_list args)
{
	/* Ignore debug-level libbpf logs */
	if (level > LIBBPF_INFO)
		return 0;
	return vfprintf(stderr, format, args);
}

void bump_memlock_rlimit(void)
{
	struct rlimit rlim_new = {
		.rlim_cur	= RLIM_INFINITY,
		.rlim_max	= RLIM_INFINITY,
	};

	check_fail(setrlimit(RLIMIT_MEMLOCK, &rlim_new),
		   "Failed to increase RLIMIT_MEMLOCK limit!");
}

static volatile bool exiting = false;

static void sig_handler(int sig)
{
	exiting = true;
}

static int handle_event(void *ctx, void *data, size_t data_sz)
{
	const struct event *e = data;
	struct tm *tm;
	char ts[32];
	time_t t;

	time(&t);
	tm = localtime(&t);
	strftime(ts, sizeof(ts), "%H:%M:%S", tm);

	printf("%-8s %-7d %-16s %s\n", ts, e->pid, e->comm, e->filename);

	return 0;
}

int main(int argc, char *argv[])
{
	int err = 0;
	struct ring_buffer *rb;
	struct check_argc0_lsm *obj;

	libbpf_set_print(libbpf_print_fn);

	/* Bump RLIMIT_MEMLOCK to create BPF maps */
	bump_memlock_rlimit();

	/* Clean handling of Ctrl-C */
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	obj = check_argc0_lsm__open_and_load();
	check_fail(!obj, "Failed to load bpf");

	rb = ring_buffer__new(bpf_map__fd(obj->maps.buffer), handle_event, NULL, NULL);
	check_fail(!rb, err = -1, cleanup, "Failed to create ring buffer");

	err = check_argc0_lsm__attach(obj);
	check_fail(err, cleanup, "Failed attach to BPF program");

	printf("%-8s %-7s %-16s %s\n",
	       "TIME", "PID", "PROCESS", "CALLING");
	while (!exiting) {
		err = ring_buffer__poll(rb, 100 /* timeout, ms */);
		/* Ctrl-C will cause -EINTR */
		if (err == -EINTR) {
			err = 0;
			break;
		}
		if (err < 0) {
			fprintf(stderr, "Error polling ring buffer: %d\n", err);
			break;
		}
	}

	check_argc0_lsm__detach(obj);

cleanup:
	ring_buffer__free(rb);
	check_argc0_lsm__destroy(obj);

	return err < 0 ? -err : 0;
}

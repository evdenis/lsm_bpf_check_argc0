#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <errno.h>

char _license[] SEC("license") = "GPL";

SEC("lsm/bprm_check_security")
int BPF_PROG(check_argc0, struct linux_binprm *bprm)
{
        if (bprm->argc == 0)
		return -EINVAL;

	return 0;
}

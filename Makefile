default: load_check_argc0_lsm trigger

vmlinux.h:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > $@

check_argc0_lsm.o: vmlinux.h check_argc0_lsm.c
	clang -g -O2 -c -target bpf check_argc0_lsm.c -o $@

check_argc0_lsm.h: check_argc0_lsm.o
	bpftool gen skeleton $< > $@

load_check_argc0_lsm: check_argc0_lsm.h load_check_argc0_lsm.c
	clang -lbpf $@.c -o $@

trigger:
	clang -w $@.c -o $@

clean:
	rm -f trigger load_check_argc0_lsm check_argc0_lsm.h check_argc0_lsm.o vmlinux.h

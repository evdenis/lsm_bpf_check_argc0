default: load_check_argc0_lsm

vmlinux.h:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > $@

check_argc0_lsm.o: vmlinux.h check_argc0_lsm.c
	clang -g -O2 -c -target bpf check_argc0_lsm.c -o $@

check_argc0_lsm.h: check_argc0_lsm.o
	bpftool gen skeleton $< > $@

load_check_argc0_lsm: check_argc0_lsm.h load_check_argc0_lsm.c
	clang -lbpf $@.c -o $@

install: load_check_argc0_lsm
	install load_check_argc0_lsm ${DESTDIR}/sbin/load_check_argc0_lsm

clean:
	rm -f load_check_argc0_lsm check_argc0_lsm.h check_argc0_lsm.o vmlinux.h

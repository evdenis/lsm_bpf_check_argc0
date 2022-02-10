![BUILD](https://github.com/evdenis/lsm_bpf_check_argc0/actions/workflows/build.yml/badge.svg)

# Check ARGC0 LSM BPF

Simple LSM BPF program to prevent program executions with argc == 0, e.g. pwnkit (CVE-2021-4034).
It does nothing more than a simple check that all exec\*() system calls are called with argc >= 1.
The check is based on Ariadne Conill's [patch](https://lore.kernel.org/all/20220127000724.15106-1-ariadne@dereferenced.org/).

## How to use

Linux kernel >= 5.8 required for BPF LSM (5.7) and BPF rignbuf (5.8).

```
# The program will start, attach an LSM BPF, and monitor the log
$ sudo ./load_check_argc0_lsm
TIME     PID     PROCESS          CALLING
12:59:21 85689   trigger          /usr/bin/pkexec
```

One can use the trigger program to test that everything works:
```
$ strace ./trigger
...
execve("/usr/bin/pkexec", NULL, NULL)   = -1 EINVAL (Invalid argument)
```

See install section for systemd service file.

## How to build

```
# On Fedora 35
$ sudo dnf install clang bpftool libbpf-devel glibc-devel.i686 glibc-devel.x86_64

$ make
```

## How to install

Run:
```
$ sudo cp load_check_argc0_lsm /usr/local/bin
$ sudo cp check_argc0_lsm.service /etc/systemd/system/
$ sudo systemctl daemon-reload
$ sudo systemctl enable check_argc0_lsm.service
$ sudo systemctl start check_argc0_lsm.service
```

Test:
```
$ strace ./trigger
...
execve("/usr/bin/pkexec", NULL, NULL)   = -1 EINVAL (Invalid argument)

$ sudo journalctl -u check_argc0_lsm.service
Feb 10 13:27:08 purple systemd[1]: Started LSM BPF protection for argc == 0 execs.
Feb 10 13:27:13 purple load_check_argc0_lsm[89177]: TIME     PID     PROCESS          CALLING
Feb 10 13:27:13 purple load_check_argc0_lsm[89177]: 13:27:13 89194   trigger          /usr/bin/pkexec
```


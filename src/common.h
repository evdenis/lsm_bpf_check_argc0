#ifndef __common_h__
#define __common_h__ 1

#define TASK_COMM_LEN 16
#define MAX_FILENAME_LEN 512

struct event {
	__u32 pid;
	char comm[TASK_COMM_LEN];
	char filename[MAX_FILENAME_LEN];
};

#endif /* __common_h__ */

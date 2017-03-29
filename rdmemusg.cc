#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "rdmemusg.h"

#define VMA_STRUCT_OBJ_SIZE 200

static char buf[2048];
static int fd = -1;

unsigned long getinfo(const char *file, const char *name) {
	int local_n;
	int found = 0;
	char *head;
	char *tail;
        unsigned long meminfo_value;

	fd = open(file, O_RDONLY);

	if (fd == -1) {
		fputs("Couldn't open meminfo file\n", stderr);
		return 1UL;
	}

	lseek(fd, 0L, SEEK_SET);

	if ((local_n = read(fd, buf, sizeof buf - 1)) < 0) {
		fputs("Couldn't read from meminfo file\n", stderr);
		return 2UL;
	}

	buf[local_n] = '\0';

	head = strstr(buf, name);

	for (;;) {
		tail = strchr(head, ':');
		if (!tail)
			break;
		*tail = '\0';
		if (!strcmp(name, (const char *)head))
			found = 1;
		head = tail + 1;
		meminfo_value = strtoul(head, &tail, 10);
		tail = strchr(head, '\n');
		if (!tail)
			break;
		if (found)
			break;
		head = tail + 1;
	}

	if (close(fd)) {
		fputs("Couldn't close meminfo file\n", stderr);
		return 3UL;
	}
	return meminfo_value;
}

unsigned long meminfo(const char *name) {
	return getinfo(MEMINFO_FILE, name);
}

unsigned long statusinfo(const char *name) {
	return getinfo("/proc/self/status", "VmRSS");
}

unsigned long slabinfo(const char *name) {
	int local_n;
	char *head;
	unsigned long active_objs;

	fd = open(SLABINFO_FILE, O_RDONLY);
	if (fd == -1) {
		fputs("Couldn't open slabinfo file\n", stderr);
		return 1UL;
	}
	lseek(fd, 0L, SEEK_SET);
	do {
		if ((local_n = read(fd, buf, sizeof buf - 1)) < 0) {
			fputs("Couldn't read from slabinfo file\n", stderr);
			return 2UL;
		}
		buf[local_n] = '\0';
		head = strstr(buf, name);
	} while (!head);
	strtok(head, " ");
	head = strtok(NULL, " ");
	if (close(fd)) {
		fputs("Couldn't close slabinfo file\n", stderr);
		return 3UL;
	}

	active_objs = strtoul(head, NULL, 10);
	//return active_objs * VMA_STRUCT_OBJ_SIZE;
	return active_objs;
}

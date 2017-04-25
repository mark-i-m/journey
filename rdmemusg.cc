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
unsigned long kswapd_total_before = 0;
unsigned long elapsed_before = 0;

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

unsigned long getkernelmem() {
	unsigned long tmem, fmem, pmem;

	tmem = meminfo("MemTotal");
	fmem = meminfo("MemFree");
	pmem = statusinfo("VmRSS");

	return tmem - fmem - pmem;
}

unsigned long get_pid_jiffies(int pid) {
    char file [40];
    sprintf(file, "/proc/%d/stat", pid);

    char buffer[2048];

	int f = open(file, O_RDONLY);
	if (f == -1) {
		fputs("Couldn't open stat file\n", stderr);
        exit(1);
	}

    int bytes_read;
    if ((bytes_read = read(f, buffer, sizeof(buffer) )) < 0) {
        printf("failed to read stat file \n"); 
        exit(1);
    }

    buffer[bytes_read] = '\0';

    char* token;
    char** my_tail = NULL;
    token = strtok(buffer, " ");

    for (int i = 0; i < 12; i++) {
        token = strtok(NULL, " ");
    }
    
    token = strtok(NULL, " ");
    unsigned long u_time = strtoul(token, my_tail, 10);
    token = strtok(NULL, " ");
    unsigned long s_time = strtoul(token, my_tail, 10);

    close(f);

    unsigned long kswapd_total_now = u_time + s_time;
    unsigned long kswapd_delta = kswapd_total_now - kswapd_total_before;
    kswapd_total_before = kswapd_total_now;

    return kswapd_delta;
}

unsigned long get_uptime_jiffies() {
    unsigned long total_uptime = 0;
    char buffer[1024];
	int f = open("/proc/stat", O_RDONLY);
	if (f == -1) {
		fputs("Couldn't open /proc/stat file\n", stderr);
        exit(1);
	}

    int bytes_read;
    if ((bytes_read = read(f, buffer, sizeof(buffer) )) < 0) {
        printf("failed to read /proc/stat file \n"); 
        exit(1);
    }
    buffer[bytes_read] = '\0';
    close(f);

    // want to get just the first line
    for (unsigned int i = 0; i < sizeof(buffer); i++) {
        if (buffer[i] == '\n'){
            buffer[i] = '\0';
            break;
        }
    }

    char** my_tail = NULL;
    char* token = strtok(buffer, " "); // first token is "cpu" 

    //  a list of various kinds of time.. 
    //  sum all of them to get total
    while (token) {
        token = strtok(NULL, " ");
        if (token != NULL)
        total_uptime += strtoul(token, my_tail, 10);
    
    }

    unsigned long elapsed_delta = total_uptime - elapsed_before;
    elapsed_before = total_uptime;

    return elapsed_delta;
}

unsigned long get_uptime() {
    char buffer[64];
	int f = open("/proc/uptime", O_RDONLY);
	if (f == -1) {
		fputs("Couldn't open uptime file\n", stderr);
        exit(1);
	}

    int bytes_read;
    if ((bytes_read = read(f, buffer, sizeof(buffer) )) < 0) {
        printf("failed to read stat file \n"); 
        exit(1);
    }
    buffer[bytes_read] = '\0';

    char** my_tail = NULL;
    char* token = strtok(buffer, " ");
    close(f);
    unsigned long uptime = strtoul(token, my_tail, 10);
    return uptime; 
}

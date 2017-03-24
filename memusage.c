#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#define MEMINFO_FILE "/proc/meminfo"

static char buf[2048];
static int meminfo_fd = -1;
unsigned long meminfo_value;

unsigned long meminfo(const char *);
unsigned long statusinfo(const char *);
unsigned long getinfo(const char *, const char *);

unsigned long getinfo(const char *file, const char *name) {
	static int local_n;
	char namebuf[16];
	int found = 0;
	char *head;
	char *tail;

	meminfo_fd = open(file, O_RDONLY);

	if (meminfo_fd == -1) {
		fputs("Couldn't open meminfo file\n", stderr);
		return 1UL;
	}

	lseek(meminfo_fd, 0L, SEEK_SET);

	if ((local_n = read(meminfo_fd, buf, sizeof buf - 1)) < 0) {
		fputs("Couldn't read from meminfo file\n", stderr);
		return 2UL;
	}

	buf[local_n] = '\0';

	head = buf;

	for (;;) {
		tail = strchr(head, ':');
		if (!tail)
			break;
		*tail = '\0';
		if (!strcmp(name, (const char *)head))
			found = 1;
		if (strlen(head) >= sizeof(namebuf)) {
			head = tail + 1;
			goto nextline;
		}
		strcpy(namebuf, head);
		head = tail + 1;
		meminfo_value = strtoul(head, &tail, 10);
	nextline:
		tail = strchr(head, '\n');
		if (!tail)
			break;
		if (found)
			break;
		head = tail + 1;
	}

	if (close(meminfo_fd)) {
		fputs("Couldn't close meminfo file\n", stderr);
		return 3UL;
	}
	return meminfo_value;
}

unsigned long meminfo(const char *name) {
	return getinfo(MEMINFO_FILE, name);
}

unsigned long statusinfo(const char *name) {
	char file[30] = { '\0' };
	strcat(file, "/proc/");
	sprintf(strchr(file, '\0'), "%d", getpid());
	strcat(file, "/status");
	return getinfo(file, "VmRSS");
}

int main(int argc, char **argv)
{
	size_t i;
	char *addr;
	unsigned long tmem, fmem, pmem, before, after;

	if (argc < 3) {
		printf("Usage: ./memusage size_order number_order\n");
		return -1;
	}

	size_t size = 1 << atoi(argv[1]);
	size_t amt = 1 << atoi(argv[2]);

	tmem = meminfo("MemTotal");
	fmem = meminfo("MemFree");
	pmem = statusinfo("VmRSS");
	before = tmem - fmem - pmem;
	printf("Diff is %lu kB\n", before);

	for (i = 0; i < amt; i++) {
		addr = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			fputs("mmap failed!\n", stderr);
			return -1;
		} else {
			addr[0] = 'A';
		}
	}

	tmem = meminfo("MemTotal");
	fmem = meminfo("MemFree");
	pmem = statusinfo("VmRSS");
	after = tmem - fmem - pmem;
	printf("Diff is %lu kB\n", after);

	printf("Growth is %lu kB\n", before - after);
	printf("hello from HelloWorld!\n");

	return 0;
}

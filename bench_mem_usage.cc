#include <stdio.h>
#include "rdmemusg.h"

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
	printf("%lu\n", before);

	for (i = 0; i < amt; i++) {
		addr = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			fputs("mmap failed!\n", stderr);
			return -1;
		} else {
	//		addr[0] = 'A';
		}

	tmem = meminfo("MemTotal");
	fmem = meminfo("MemFree");
	pmem = statusinfo("VmRSS");
	after = tmem - fmem - pmem;
	printf("%lu\n", after);

	}

	tmem = meminfo("MemTotal");
	fmem = meminfo("MemFree");
	pmem = statusinfo("VmRSS");
	after = tmem - fmem - pmem;
	printf("%lu\n", after);

	return 0;
}

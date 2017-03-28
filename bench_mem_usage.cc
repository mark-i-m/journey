#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "rdmemusg.h"

int main(int argc, char **argv)
{
	size_t i;
	char *addr;

	if (argc < 3) {
		printf("Usage: ./memusage size_order number_order\n");
		return -1;
	}

	size_t size = 1 << atoi(argv[1]);
	size_t amt = 1 << atoi(argv[2]);

	printf("%lu %lu %lu\n", getkernelmem(), slabinfo("vm_area_struct"), meminfo("PageTables"));

	for (i = 0; i < amt; i++) {
		addr = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			fputs("mmap failed!\n", stderr);
			return -1;
		} else {
	//		addr[0] = 'A';
		}

        printf("%lu %lu %lu\n", getkernelmem(), slabinfo("vm_area_struct"), meminfo("PageTables"));
	}
}

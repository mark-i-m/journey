#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "rdmemusg.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: ./memusage number_order\n");
		return -1;
	}

	size_t amt = 1 << atoi(argv[1]);

	for (int i = 0; i < amt; i++) {
		printf("%lu %lu %lu\n", getkernelmem(), slabinfo("vm_area_struct"), meminfo("PageTables"));
	}
}

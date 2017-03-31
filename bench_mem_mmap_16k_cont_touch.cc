#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "rdmemusg.h"
#include "pattern.h"

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Usage: ./memusage size_order number_order\n");
		return -1;
	}

	size_t size = 1 << atoi(argv[1]);
	size_t amt = 1 << atoi(argv[2]);

    PatternGenerator pg = get_mmap_16k_cont_rwx();
    Next next;

	printf("%lu %lu %lu\n", getkernelmem(), slabinfo("vm_area_struct"), meminfo("PageTables"));

    for (size_t i = 0; i < amt; i++) {
        next = pg.next();

        char *addr = (char *)mmap(next.address, next.size, next.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
        if (addr == MAP_FAILED) {
            fputs("mmap failed!\n", stderr);
            return -1;
        } else {
           	addr[0] = 'A';
        }

        printf("%lu %lu %lu\n", getkernelmem(), slabinfo("vm_area_struct"), meminfo("PageTables"));
    }
}

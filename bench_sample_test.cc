#include <stdio.h>
#include <stdlib.h>

#include "pattern.h"
#include "rdmemusg.h"

#include <sys/mman.h>

int main(int argc, char **argv) {
        PatternGenerator p = get_frag_prot_pattern();
        Next val;
        size_t i;

	char *addr;
	unsigned long tmem, fmem, pmem, before, after;

	if (argc < 2) {
		printf("Usage: ./pattern number_order\n");
		return -1;
	}

	size_t amt = 1 << atoi(argv[1]);

	tmem = meminfo("MemTotal");
	fmem = meminfo("MemFree");
	pmem = statusinfo("VmRSS");
	before = tmem - fmem - pmem;
	printf("%lu\n", before);

        for (i = 0; i < amt; i++) {
          val = p.next();
          if (val.operation == OP_MMAP) {
            addr = (char *)mmap(val.address, val.size, val.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
            if (addr == MAP_FAILED) {
              fputs("mmap failed!\n", stderr);
              return -1;
            }
          } else if (val.operation == OP_MPROT) {
            int j = mprotect(val.address, val.size, val.permissions);
            if (j) {
              fputs("mprotect failed!\n", stderr);
              return -1;
            }
          }
           		addr[0] = 'A';

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

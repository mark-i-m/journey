#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>

#include "rdmemusg.h"
#include "pattern.h"

int main(int argc, char **argv)
{
	if (argc < 1) {
        std::cout << "Usage: ./memusage number_order" << std::endl;
		return -1;
	}

	size_t amt = 1 << atoi(argv[1]);

    PatternGenerator pg = get_mmap_16k_cont_rwx();
    Next next;

    std::cout << getkernelmem() << " " 
              << slabinfo("vm_area_struct") << " " 
              << meminfo("PageTables") << std::endl;

    for (size_t i = 0; i < amt; i++) {
        next = pg.next();

        char *addr = (char *)mmap(next.address, next.size, next.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
        if (addr == MAP_FAILED) {
            std::cerr << "mmap failed: " << addr << std::endl;
            return -1;
        } else {
           	addr[0] = 'A';
        }

        std::cout << getkernelmem() << " " 
                  << slabinfo("vm_area_struct") << " " 
                  << meminfo("PageTables") << std::endl;
    }
}

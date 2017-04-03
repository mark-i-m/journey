#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>

#include "rdmemusg.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
        std::cout << "Usage: ./memusage number_order" << std::endl;
		return -1;
	}

	size_t amt = 1 << atoi(argv[1]);

    std::cout << getkernelmem() << " " 
              << slabinfo("vm_area_struct") << " " 
              << meminfo("PageTables") << std::endl;

	for (size_t i = 0; i < amt; i++) {
        std::cout << getkernelmem() << " " 
                  << slabinfo("vm_area_struct") << " " 
                  << meminfo("PageTables") << std::endl;
	}
}

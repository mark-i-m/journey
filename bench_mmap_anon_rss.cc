
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "rdmemusg.h"
#include "pattern.h"
#include "pinning.h"

int main(int argc, char **argv) {
    set_cpu(0);


	if (argc < 1) {
        std::cout << "Usage: ./memusage number_order" << std::endl;
        // whatever!
		return -1;
	}

    uint64_t vma_size = (uint64_t)1 << 35; // fixed at 32 GB

    char* start_addr = (char*)mmap(0, vma_size, 7, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (start_addr == MAP_FAILED) {
       printf("fuck\n");
       exit(1);
    }

    unsigned long long num_pages_mapped = vma_size / PAGE_SIZE;
    unsigned long* res_array = (unsigned long*)malloc(num_pages_mapped * 
            sizeof(unsigned long));
    // lock this results array itself so itself is not swapped out
    mlock((const void*) res_array, num_pages_mapped * sizeof(unsigned long));

    
    // touch all the pages now
    unsigned long long num_touched = 0;
    char* addr = start_addr;
    while (num_touched < num_pages_mapped) {
        addr[0] = 'X';
        res_array[num_touched++] = statusinfo("VmRSS");

        addr += PAGE_SIZE;
    }

    for (unsigned int i = 0; i < num_pages_mapped; i++) std::cout << res_array[i++] << std::endl;

}

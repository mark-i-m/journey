
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

	if (argc < 3) {
        std::cout << "Usage: ./prog num_gb kswapd_pid" << std::endl;
		return -1;
	}

	size_t gbs = atoi(argv[1]);
	size_t kswapd = atoi(argv[2]);
    uint64_t vma_size = (uint64_t)gbs << 30;

    char* start_addr = (char*)mmap(NULL, vma_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (start_addr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    unsigned long long num_pages_mapped = vma_size / PAGE_SIZE;
    double* res_array = (double*)malloc(num_pages_mapped * sizeof(double));

    // touch all pages
    for(unsigned long i = 0; i < num_pages_mapped; i++) {
        res_array[i] = 0;
    }

    // lock this results array itself so itself is not swapped out
    if(mlock((const void*) res_array, num_pages_mapped * sizeof(double))) {
        std::cerr << "mlock failed: " << strerror(errno) << std::endl;
        exit(1);
    }
    
    // touch all the pages now
    unsigned long long num_touched = 0;
    char* addr = start_addr;
    while (num_touched < num_pages_mapped) {
        *addr = 'X';
        res_array[num_touched++] = get_CPU_usage(kswapd);

        addr += PAGE_SIZE;
    }

    for (unsigned int i = 0; i < num_pages_mapped; i++) {
        std::cout << res_array[i] << std::endl;
    }
}

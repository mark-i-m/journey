
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "rdtsc.h"
#include "pattern.h"
#include "pinning.h"
#include "rdmemusg.h"
int main(int argc, char **argv) {
    set_cpu(0);

	if (argc < 3) {
        std::cout << "Usage: ./memusage num_gb pid_of_kswapd0" << std::endl;
		return -1;
	}


	size_t gbs = atoi(argv[1]);
    int kswapd_pid = atoi(argv[2]);
    uint64_t vma_size = (uint64_t)gbs << 30;

    char* start_addr = (char*)mmap(NULL, vma_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (start_addr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    unsigned long long num_pages_mapped = vma_size / PAGE_SIZE;
    unsigned long long* res_array = (unsigned long long*)malloc(num_pages_mapped * 
            sizeof(unsigned long long));

    // touch all pages
    for(unsigned long i = 0; i < num_pages_mapped; i++) {
        res_array[i] = 0;
    }

    // lock this results array itself so it is not swapped out
    if(mlock((const void*) res_array, num_pages_mapped * sizeof(unsigned long long))) {
        std::cerr << "mlock failed: " << strerror(errno) << std::endl;
        exit(1);
    }
    
    // touch all the pages now
    unsigned long long num_touched = 0;
    char* addr = start_addr;
    while (num_touched < num_pages_mapped) {
        unsigned long long start = rdtsc();
        *addr = 'X';
        res_array[num_touched++] = rdtsc() - start;
        if (num_touched % 80000 == 0)  // how to set this number? if this is too short elapsed ~= 0 
        printf("kswapd cpu util %lu \n", get_CPU_usage(kswapd_pid));

        addr += PAGE_SIZE;
    }

    for (unsigned int i = 0; i < num_pages_mapped; i++) {
        std::cout << res_array[i] << std::endl;
    }
}

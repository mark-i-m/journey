
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include <cstdlib>
#include <ctime>

#include "rdmemusg.h"
#include "pattern.h"
#include "pinning.h"

int main(int argc, char **argv) {
    set_cpu(0);

    uint64_t vma_size = (uint64_t)1 << 35; // fixed at 32 GB

    char* start_addr = (char*)mmap(NULL, vma_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (start_addr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    unsigned long long num_pages_mapped = vma_size / PAGE_SIZE;
    unsigned long* res_array = (unsigned long*)malloc(num_pages_mapped * 
            sizeof(unsigned long));

    // lock this results array itself so itself is not swapped out
    if(!mlock((const void*) res_array, num_pages_mapped * sizeof(unsigned long))) {
        std::cerr << "mlock failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    // seed the rng
	srand((int)time(0));
    
    // touch all the pages now
    unsigned long long num_touched = 0;
    char* addr = start_addr;
    while (num_touched < num_pages_mapped) {
        *addr = 'X';
        res_array[num_touched++] = statusinfo("VmRSS");

        addr = ((char*)start_addr) + (rand() % vma_size);
    }

    for (unsigned int i = 0; i < num_pages_mapped; i++) {
        std::cout << res_array[i++] << std::endl;
    }
}

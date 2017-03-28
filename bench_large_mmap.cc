#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <sys/mman.h>

#include "rdtsc.h"

#define WARMUP 100

int main(int argc, char **argv) {
    // how much to mmap?
    if(argc < 2) {
        printf("Usage: ./large_mmap order\n");
        return -1;
    }

    size_t amt = 1L << atoi(argv[1]);

    // TODO: try mprotect
    // TODO: try with huge pages
    
    
    // warmup
    for(int i = 0; i < WARMUP; i++) {
        char *addr = (char*)mmap(NULL, amt, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        if(addr == MAP_FAILED) {
            printf("mmap failed: %s\n", strerror(errno));
        }
    }

    // actual
    unsigned long long start = rdtsc();

    char *addr = (char*)mmap(NULL, amt, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    unsigned long long time = rdtsc() - start;

    if(addr == MAP_FAILED) {
        printf("mmap failed: %s\n", strerror(errno));
        return -1;
    }

    /*
    // touch all pages
    for(long i = 0; i < SIZE; i += (4 << 10)) {
        *(addr + i) = 'A';
    }
    */

    printf("%llu\n", time);
}

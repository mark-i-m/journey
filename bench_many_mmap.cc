#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "rdtsc.h"

int main(int argc, char **argv) {
    // number and size of each mmap
    if(argc < 3) {
        printf("Usage: ./large_mmap order number\n");
        return -1;
    }

    printf("%d\n\n", (int)getpid());

    size_t amt = 1L << atoi(argv[1]);
    size_t number = atol(argv[2]);

    // actual
    for(int i = 0; i < number; i++) {
        unsigned long long start = rdtsc();

        char *addr = (char*)mmap(NULL, amt, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

        unsigned long long time = rdtsc() - start;

        if(addr == MAP_FAILED) {
            printf("mmap failed: %s\n", strerror(errno));
            return -1;
        }

        printf("%llu\n", time);
    }

    /*
    // touch all pages
    for(long i = 0; i < SIZE; i += (4 << 10)) {
        *(addr + i) = 'A';
    }
    */
}

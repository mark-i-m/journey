#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <sys/mman.h>

#include "rdtsc.h"


int main(int argc, char **argv) {
    // how much to mmap?
    if(argc < 3) {
        printf("Usage: ./many_vma size_order number_order\n");
        return -1;
    }

    size_t size = 1L << atoi(argv[1]);
    size_t amt = 1L << atoi(argv[2]);

    /*
     * Idea: if we create VMAs with alternating permissions, they cannot be
     * merged together. i.e. the kernel will have to keep creating different
     * VMAs, which is slow.
     *
     * To prevent it from creating just two VMAs and expanding them, we provide
     * an address hint!
     */

#define HINT ((void*) 0x80000000)
#define PLUS(i) ((void*)(((char*)HINT) + ((i)*size)))

    // actual
    for(size_t i = 0; i < amt; i++) {
        unsigned long long start = rdtsc();

        void *addr;
 
        //if(i % 2) {
            addr = mmap(PLUS(i), size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        //} else {
        //    addr = mmap(PLUS(i), size, PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);
        //}

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

    //printf("%llu\n", time);
}

#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "rdmemusg.h"
#include "pattern.h"
#include "rdtsc.h"
#include "pinning.h"

int main(int argc, char **argv) {
    set_cpu(0);

    if (argc < 2) {
      std::cout << "Usage: ./memusage number_order" << std::endl;
      return -1;
    }

    size_t amt = 1 << atoi(argv[1]);

    PatternGenerator pg = get_mmap_cont(1 /* 4KB */, 7 /* RWX */);
    Next next;

    for (size_t i = 0; i < amt; i++) {
        next = pg.next();

        char *addr = (char *)mmap(next.address, next.size, next.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);

        if (addr == MAP_FAILED) {
            std::cerr << "mmap failed: " << strerror(errno) << std::endl;
            return -1;
        }
    }

    pg = get_mremap_frag(2, 4, 7);

    for (size_t i = 0; i < amt; i++) {
        next = pg.next();

        unsigned long long cur_start = rdtsc();
        char *addr = (char *)mremap(next.address, 1 << 12, next.size, MREMAP_MAYMOVE);
        unsigned long long cur_end = rdtsc();

        if (addr == MAP_FAILED) {
            std::cerr << "mmap failed: " << strerror(errno) << std::endl;
            return -1;
        }
        std::cout << (cur_end - cur_start) << std::endl;
    }
}

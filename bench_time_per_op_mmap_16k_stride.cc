#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include "pattern.h"
#include "rdtsc.h"
#include "pinning.h"

int main(int argc, char* argv[]) {
    set_cpu(0);
    PatternGenerator pg = get_mmap_stride(1 /* 4KB */, 7 /* RWX */, 2 /* 8KB */);

    Next cur;
    char* addr;

    if (argc < 2) {
        std::cout << "Usage: ./pattern total_ops_order" << std::endl;
        exit(1);
    }

    int tot_ops = 1 << atoi(argv[1]);
    
    for (int i = 0; i < tot_ops; i++) {
        cur = pg.next();
        unsigned long long start = rdtsc();
        addr = (char *)mmap(cur.address, cur.size, cur.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
        unsigned long long end = rdtsc();
        if (addr == MAP_FAILED) {
            std::cerr << "mmap failed: " << strerror(errno) << std::endl;
            exit(1);
        }

        std::cout << (end - start) << std::endl;
    }

    return 0;
}


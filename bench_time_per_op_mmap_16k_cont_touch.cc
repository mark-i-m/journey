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
    PatternGenerator pg = get_mmap_cont(4 /* 16KB */, 7 /* RWX */);

    Next cur;
    size_t i;
    char* addr;

    if (argc < 2) {
        std::cout << "Usage: ./pattern total_ops_order" << std::endl;
        exit(1);
    }

    int tot_ops = 1 << atoi(argv[1]);
    
    double ticks_per_nano = get_ticks_per_nano();

    unsigned long long start = rdtsc();
    
    for (int i = 0; i < tot_ops; i++) {
        //  assuming all ops were sucessful
        cur = pg.next();
        if (cur.operation == OP_MMAP) {  // must be
            addr = (char *)mmap(cur.address, cur.size, cur.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
            if (addr == MAP_FAILED) {
                std::cerr << "mmap failed: " << strerror(errno) << std::endl;
                exit(1);
            }
        }
        addr[0] = 'X'; // touch
    }

    unsigned long long end = rdtsc();
    unsigned long long interval = end - start;
    unsigned long long nano_secs = (double) interval / ticks_per_nano;

    // nsec per operation
    std::cout <<  nano_secs / tot_ops << std::endl;

    return 0;

}

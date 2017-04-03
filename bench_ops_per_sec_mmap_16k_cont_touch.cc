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
    //size_t i;
    char* addr;

    if (argc < 2) {
        std::cout << "Usage: ./pattern total_time_sec" << std::endl;
        exit(1);
    }

    int time_to_run = atoi(argv[1]);
    
    double ticks_per_nano = get_ticks_per_nano();


    unsigned long long total_ticks_todo = (unsigned long long)ticks_per_nano * NANO_SEC_PER_SEC * 
        (unsigned long long)time_to_run;


    unsigned int ops_done = 0;
    unsigned long long start = rdtsc();

    while (rdtsc() - start < total_ticks_todo) {
        cur = pg.next();
        if (cur.operation == OP_MMAP) {  // must be
            addr = (char *)mmap(cur.address, cur.size, cur.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
            if (addr == MAP_FAILED) {
                std::cerr << "mmap failed: " << strerror(errno) << std::endl;
                exit(1);
            }
        }
        ops_done++;
        addr[0] = 'X'; // touch
    }

    // operations per second
    std::cout <<  ops_done / time_to_run << std::endl;

    return 0;

}

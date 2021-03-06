#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#include "pattern.h"
#include "rdtsc.h"
#include "pinning.h"

int main(int argc, char* argv[]) {
    set_cpu(0);
    PatternGenerator pg = get_mmap_small_remap_large(1 /* 4KB */,  7 /* RWX */, 4 /* 16KB*/);

    Next cur;
    char* addr;

    if (argc < 2) {
        printf("Usage: ./pattern total_ops_order\n");
        exit(1);
    }

    int tot_ops = 1 << atoi(argv[1]);
    
    double ticks_per_nano = get_ticks_per_nano();

    unsigned long long start = rdtsc();
    
    unsigned int size;
    for (int i = 0; i < tot_ops; i++) {
        //  assuming all ops were sucessful
        cur = pg.next();
        if (cur.operation == OP_MMAP) {  // must be
            size = cur.size;  
            addr = (char *)mmap(cur.address, cur.size, cur.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
            if (addr == MAP_FAILED) {
                printf("mmap failed\n");
                exit(1);
            }
        } else if (cur.operation == OP_MREMAP) {
            assert(cur.size != size); //  make sure we use the size from last time. 
            addr = (char*) mremap(addr /* old addr */, size /* old size */, cur.size /* new size*/, 
                    MREMAP_MAYMOVE);
            
        
        }
        addr[0] = 'X'; // touch
    }

    unsigned long long end = rdtsc();
    unsigned long long interval = end - start;
    unsigned long long nano_secs = (double) interval / ticks_per_nano;

    printf("nsec/op = %llu\n", nano_secs / tot_ops);

    return 0;

}

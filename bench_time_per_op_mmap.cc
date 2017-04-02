#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "pattern.h"
#include "rdtsc.h"
#include "pinning.h"

int main(int argc, char* argv[]) {
    set_cpu(0);
    PatternGenerator pg = get_mmap_cont(1, 7);

    Next cur;
    char* addr;

    if (argc < 3) {
        printf("Usage: ./pattern total_ops_order touch_0_or_1\n");
        exit(1);
    }

    int tot_ops = 1 << atoi(argv[1]);
    int touch = atoi(argv[2]);
    
    double ticks_per_nano = get_ticks_per_nano();  // calibrate rdtsc before measuring

    
    for (int i = 0; i < tot_ops; i++) {
        //  assuming all ops were sucessful
        cur = pg.next();
        if (cur.operation == OP_MMAP) {  // must be
            unsigned long long cur_start = rdtsc();
            addr = (char *)mmap(cur.address, cur.size, cur.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
            unsigned long long cur_end = rdtsc();
            if (addr == MAP_FAILED) {
                printf("mmap failed\n");
                exit(1);
            }
            print_interval(cur_start, cur_end, ticks_per_nano);
        }
        if (touch == 1) addr[0] = 'X'; // touch
    }

/*    unsigned long long end = rdtsc();
    unsigned long long interval = end - start;
    unsigned long long nano_secs = (double) interval / ticks_per_nano;*/

/*    printf("nsec/op = %llu\n", nano_secs / tot_ops);*/

    return 0;

}

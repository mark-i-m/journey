#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "pattern.h"
#include "rdtsc.h"
#include "pinning.h"

int main(int argc, char* argv[]) {
    set_cpu(0);
    PatternGenerator pg = get_continuous_pattern();

    Next cur;
    size_t i;
    char* addr;

    if (argc < 2) {
        printf("Usage: ./pattern total_time_sec\n");
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
                printf("mmap failed\n");
                exit(1);
            }
        }
        ops_done++;
        addr[0] = 'X'; // touch
    }

    printf("operations/sec = %d\n", ops_done / time_to_run);

}

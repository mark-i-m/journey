#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "rdmemusg.h"
#include "pattern.h"
#include "rdtsc.h"
#include "pinning.h"

#define START 0x40000000

int main(int argc, char **argv) {
    set_cpu(0);

    if (argc < 2) {
      std::cout << "Usage: ./memusage number_order" << std::endl;
      return -1;
    }

    size_t amt = 1 << atoi(argv[1]);

    // Read randomized offsets from a file
    FILE *f = fopen("offsets.txt", "r");
    int *offsets;
    unsigned int i;

    if (!f) {
      fputs("Couldn't open offsets.txt\n", stderr);
      return -1;
    }

    offsets = (int *)malloc(sizeof(int) * amt);

    for (i = 0; i < amt; i++) {
      fscanf(f, "%d", &offsets[i]);
    }

    if (fclose(f)) {
      fputs("Couldn't close offsets.txt\n", stderr);
      free(offsets);
      return -1;
    }

    double ticks_per_nano = get_ticks_per_nano();  // calibrate rdtsc before measuring

    for (size_t i = 0; i < amt; i++) {
        unsigned long maddr = START + (offsets[i] << 12);
        unsigned long long cur_start = rdtsc();
        char *addr = (char *)mmap((void *)maddr, 4 << 12, 7, MAP_ANON | MAP_PRIVATE, -1, 0);
        unsigned long long cur_end = rdtsc();
        if (addr == MAP_FAILED) {
            std::cerr << "mmap failed: " << strerror(errno) << std::endl;
            return -1;
        }
        print_interval(cur_start, cur_end, ticks_per_nano);
    }

    free(offsets);

    return 0;
}

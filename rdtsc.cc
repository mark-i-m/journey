#include "rdtsc.h"


double get_ticks_per_nano() {
   struct timespec start;
   struct timespec end;
   clock_gettime(CLOCK_MONOTONIC, &start);
   unsigned long long r_start = rdtsc();
   for (int i = 0; i < 10000000; i++);
   unsigned long long total_ticks = rdtsc() - r_start;
   clock_gettime(CLOCK_MONOTONIC, &end);
   unsigned long long interval = NANO_SEC_PER_SEC * (end.tv_sec - start.tv_sec) 
       + end.tv_nsec - start.tv_nsec;
   double ticks_per_nano = (double) total_ticks / (double) interval;
}



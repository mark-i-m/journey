#ifndef _RDTSC_H_
#define _RDTSC_H_
#include <time.h>

const unsigned long long NANO_SEC_PER_SEC = 1e9;

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

double get_ticks_per_nano();
void print_interval(unsigned long long, unsigned long long, double);
  



#endif

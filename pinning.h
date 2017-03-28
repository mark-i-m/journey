#ifndef _PINNING_H_
#define _PINNING_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sched.h>
#include <stdio.h>

#define CPU_PARENT 1
#define CPU_CHILD 2

int set_cpu(int cpu) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);

    return sched_setaffinity(0, sizeof(cpu_set_t), &set);
}

#endif

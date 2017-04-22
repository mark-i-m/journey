#ifndef _RDMEMUSAGE_H_
#define _RDMEMUSAGE_H_

#define MEMINFO_FILE "/proc/meminfo"
#define SLABINFO_FILE "/proc/slabinfo"

unsigned long getinfo(const char *file, const char *name);
unsigned long meminfo(const char *name);
unsigned long statusinfo(const char *name);
unsigned long slabinfo(const char *name);
double get_CPU_usage(int);
unsigned long get_uptime();
unsigned long get_uptime_jiffies();

unsigned long getkernelmem();

#endif

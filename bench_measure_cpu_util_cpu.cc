#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

#include "rdtsc.h"
#include "pattern.h"
#include "pinning.h"
#include "rdmemusg.h"

void on_stop(int signum) {
    //std::cout << "bye!" << std::endl;
    exit(0);
}


int main(int argc, char **argv) {
    set_cpu(1);  //  run it on a different cpu from the actual benchmark. no context switches

    signal(SIGINT, on_stop);

	if (argc < 2) {
        std::cout << "Usage: ./executable pid_of_process" << std::endl;
		return -1;
	}

    int kswapd_pid = atoi(argv[1]);

    // touch all the pages now
    while (1) {
        std::cout << get_CPU_usage(kswapd_pid) << std::endl;
        usleep(500);  // I don't know how to tune this time. It makes a difference. 
    }

}

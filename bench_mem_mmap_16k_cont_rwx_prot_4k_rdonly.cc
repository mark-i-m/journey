#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include "pattern.h"
#include "rdmemusg.h"
#include "pinning.h"

int main(int argc, char **argv) {
    set_cpu(0);

    if (argc < 2) {
        std::cout << "Usage: ./pattern number_order" << std::endl;
        return -1;
    }

    size_t amt = 1 << atoi(argv[1]);

    PatternGenerator p = get_mmap_cont_change_prot_partial( 4 /* 16 KB */,
                                7 /* RWX */, 1 /* 4KB */, 4 /* R */);
    Next val;

    std::cout << getkernelmem() << " " 
              << slabinfo("vm_area_struct") << " " 
              << meminfo("PageTables") << std::endl;

    for (size_t i = 0; i < amt; i++) {
        char *addr;
        val = p.next();

        if (val.operation == OP_MMAP) {
            addr = (char *)mmap(val.address, val.size, val.permissions, MAP_ANON | MAP_PRIVATE, -1, 0);
            if (addr == MAP_FAILED) {
                std::cerr << "mmap failed: " << strerror(errno) << std::endl;
                return -1;
            }
        } else if (val.operation == OP_MPROT) {
            int j = mprotect(val.address, val.size, val.permissions);
            if (j) {
                std::cerr << "mprotect failed: " << strerror(errno) << std::endl;
                return -1;
            }
        }

        std::cout << getkernelmem() << " " 
                  << slabinfo("vm_area_struct") << " " 
                  << meminfo("PageTables") << std::endl;
    }
}

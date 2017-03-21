#include "pattern.h"

#include <sys/mman.h>

PatternGenerator::PatternGenerator (std::vector<PatternPart> pattern, void *start) : 
    pattern(pattern), index(0) {
        previous.address = start;
}

Next PatternGenerator::next() {
    Next next;

    next.size = pattern[index].size;
    next.operation = pattern[index].operation;
    next.address = get_address();
    next.permissions = get_prot();

    previous = next;

    index = (index + 1) % pattern.size();

    return next;
}

void *PatternGenerator::get_address() {
    unsigned long prev = (unsigned long)previous.address;
    unsigned long next = prev + pattern[index].location * PAGE_SIZE;

    return (void*)next;
}

int PatternGenerator::get_prot() {
    char prot = pattern[index].permissions;

    switch(pattern[index].operation) {
        case OP_MMAP:
        case OP_MPROT:
            return prot ?
                   (prot & 1 ? PROT_EXEC : 0) | 
                   (prot & 2 ? PROT_WRITE : 0) |
                   (prot & 4 ? PROT_READ : 0) :
                    PROT_NONE;

        default:
            return 0;
    }
}

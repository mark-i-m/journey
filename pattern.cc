#include "pattern.h"

#include <sys/mman.h>

#include <iostream>

void Next::print() {
    std::cout << "op: " << operation << ", addr: 0x" << std::hex << 
        address << std::dec << ", size: " << size << ", permissions: " << 
        permissions << std::endl;
}

PatternGenerator::PatternGenerator (std::vector<PatternPart> pattern, void *start) : 
    pattern(pattern), index(0) {
        previous.address = start;
}

Next PatternGenerator::next() {
    Next next;

    next.size = pattern[index].size * PAGE_SIZE;
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

// mmap a bunch of 16KB regions that are contiguous
PatternGenerator get_mmap_16k_cont_rwx() {
    PatternPart only;
    only.size = 4; // 16KB
    only.location = 4;
    only.permissions = 7;
    only.operation = OP_MMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(only);

    return PatternGenerator(pattern);
}

// mmap a bunch of large contiguous pieces of memory
PatternGenerator get_mmap_1M_cont_rwx() {
    PatternPart only;
    only.size = 256; // 1MB
    only.location = 256;
    only.permissions = 7;
    only.operation = OP_MMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(only);

    return PatternGenerator(pattern);
}

// mmap some space, then change permissions on part of it
PatternGenerator get_frag_prot_pattern() {
    PatternPart first;
    first.size = 8; // 32KB
    first.location = 4;
    first.permissions = 7;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = 4; // 16KB
    second.location = 4;
    second.permissions = 5;
    second.operation = OP_MPROT;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

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

// mmap a bunch of pages that are contiguous
PatternGenerator get_mmap_cont(int size, char permissions) {
    PatternPart only;
    only.size = size; // size * 4KB
    only.location = size;
    only.permissions = permissions;
    only.operation = OP_MMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(only);

    return PatternGenerator(pattern);
}

// mmap a bunch of pages sequentially with a given stride
PatternGenerator get_mmap_stride(int size, char permissions, int stride) {
    PatternPart only;
    only.size = size; // size * 4KB
    only.location = stride;
    only.permissions = permissions;
    only.operation = OP_MMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(only);

    return PatternGenerator(pattern);
}

// mmap a bunch of pages contiguously and change their permissions
PatternGenerator get_mmap_cont_change_prot(int size, char permissions, char new_permissions) {
    PatternPart first;
    first.size = size; // size * 4KB
    first.location = size;
    first.permissions = permissions;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = size; // size * 4KB
    second.location = 0;
    second.permissions = new_permissions;
    second.operation = OP_MPROT;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

// mmap a bunch of pages contiguosly and change permissions of subset of these pages
PatternGenerator get_mmap_cont_change_prot_partial(int size, char permissions,\
                int sub_size, char new_permissions) {
    PatternPart first;
    first.size = size; // size * 4KB
    first.location = size - sub_size;
    first.permissions = permissions;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = sub_size; // sub_size * 4KB
    second.location = 0;
    second.permissions = new_permissions;
    second.operation = OP_MPROT;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

// mmap a bunch of pages
// Resize them to smaller pages
PatternGenerator get_mmap_large_remap_small(int large_size, char permissions, int small_size) {
    PatternPart first;
    first.size = large_size; // large_size * 4KB
    first.location = small_size;
    first.permissions = permissions;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = small_size; // small_size * 4KB
    second.location = 0;
    second.permissions = permissions;
    second.operation = OP_MREMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

// mmap a bunch of 4KB pages
// Resize them to 16KB pages
PatternGenerator get_mmap_small_remap_large(int small_size, char permissions, int large_size) {
    PatternPart first;
    first.size = small_size; // small_size * 4KB
    first.location = large_size;
    first.permissions = permissions;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = large_size; // large_size * 4KB
    second.location = 0;
    second.permissions = permissions;
    second.operation = OP_MREMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

// mmap a bunch of 4KB pages with a stride of 8KB
// Remap the 4KB pages to the unmapped spaces
PatternGenerator get_mmap_stride_reloc(int size, char permissions) {
    PatternPart first;
    first.size = size; // size * 4KB
    first.location = size;
    first.permissions = permissions;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = size; // size * 4KB
    second.location = size;
    second.permissions = permissions;
    second.operation = OP_MREMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

// mmap a bunch of 16KB pages contiguosly
// Unmap 8KB pages in the middle of these 16KB pages
PatternGenerator get_mmap_16k_cont_munmap_8k() {
    PatternPart first;
    first.size = 4; // 16KB
    first.location = 3;
    first.permissions = 7;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = 2; // 4KB
    second.location = 1;
    second.permissions = 7;
    second.operation = OP_MUNMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

// mmap a bunch of 4KB pages contiguously
// Resize them to 8KB pages
// Unmap the initial 4KB
PatternGenerator get_mmap_4k_remap_8k_unmap_4k() {
    PatternPart first;
    first.size = 1; // 4KB
    first.location = 2;
    first.permissions = 7;
    first.operation = OP_MMAP;

    PatternPart second;
    second.size = 2; // 8KB
    second.location = 0;
    second.permissions = 7;
    second.operation = OP_MREMAP;

    PatternPart third;
    third.size = 1; // 4KB
    third.location = 0;
    third.permissions = 7;
    third.operation = OP_MUNMAP;

    std::vector<PatternPart> pattern;
    pattern.push_back(first);
    pattern.push_back(second);

    return PatternGenerator(pattern);
}

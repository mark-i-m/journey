#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <stddef.h>
#include <vector>

// A bunch of flags representing supported operations
#define OP_MMAP 100
#define OP_MPROT 101
#define OP_MADV 102
#define OP_MLOCK 103
#define OP_MREMAP 104

#define STARTING_POINT ((void*)0x40000000) // the first address used for calculation

#define PAGE_SIZE (4 << 10) // Normal page (in B) on x86 Linux

struct PatternPart {
    int location; // Relative location from start of previous region (in pages)
    char permissions; // 00000RWE <- bits
    size_t size; // pages
    int operation; // one of the flags defined above
};

struct Next {
    int operation; // one of the flags defined above
    void *address; // the address for the start of the region
    size_t size; // bytes
    int permissions; // usable by the given operation

    void print();
};

class PatternGenerator {
    private:
        std::vector<PatternPart> pattern;
        size_t index;

        Next previous; // the last "next" operation returned

        void *get_address();
        int get_prot();

    public:
        // Create a generator with the given pattern and starting point
        PatternGenerator(std::vector<PatternPart>, void *start = STARTING_POINT);
        Next next();
};

// Define a bunch of common patterns

// mmap a bunch of 16KB regions that are contiguous
PatternGenerator get_continuous_pattern();

// mmap a bunch of large contiguous pieces of memory
PatternGenerator get_large_continuous_pattern();

// mmap some space, then change permissions on part of it
PatternGenerator get_frag_prot_pattern();

#endif

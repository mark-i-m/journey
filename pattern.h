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
#define OP_MUNMAP 105

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
PatternGenerator get_mmap_cont(int, char);
PatternGenerator get_mmap_cont_change_prot(int, char, char);
PatternGenerator get_mmap_cont_change_prot_partial(int, char, int, char);
PatternGenerator get_mmap_stride(int, char, int);
PatternGenerator get_mmap_large_remap_small(int, char, int);
PatternGenerator get_mmap_small_remap_large(int, char, int);
PatternGenerator get_mremap_frag(unsigned long, int, char);
PatternGenerator get_mmap_stride_reloc(int, char);
PatternGenerator get_mmap_16k_cont_munmap_8k();
PatternGenerator get_mmap_4k_remap_8k_unmap_4k();

#endif

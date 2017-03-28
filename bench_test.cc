#include "pattern.h"
#include <stdio.h>

int main() {
    PatternGenerator p1 = get_frag_prot_pattern();

    p1.next().print();
    p1.next().print();
    p1.next().print();
    p1.next().print();
    p1.next().print();
    p1.next().print();
    p1.next().print();
    p1.next().print();
}

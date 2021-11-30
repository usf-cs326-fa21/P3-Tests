#include <stdlib.h>
#include "vmsize.h"
#include "../../allocator.c"

int main(void)
{
    void *a = malloc_impl(120,   "Allocation [A]");
    void *b = malloc_impl(345,   "Allocation [B]");
    void *c = malloc_impl(15,    "Allocation [C]");
    void *d = malloc_impl(3,     "Allocation [D]");
    void *e = malloc_impl(10999, "Allocation [E]");
    void *f = malloc_impl(894,   "Allocation [F]");
    void *g = malloc_impl(99999, "Allocation [G]");

    print_memory();

    return 0;
}

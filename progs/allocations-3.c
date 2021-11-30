#include <stdlib.h>
#include "../../allocator.c"

int main(void)
{
    void *a = malloc_impl(500, "Test Allocation: 0");
    void *b = malloc_impl(294, "Test Allocation: 1");
    void *c = malloc_impl(250, "Test Allocation: 2");
    void *d = malloc_impl(1000, "Test Allocation: 3");
    void *e = malloc_impl(400, "Test Allocation: 4");

    free_impl(b);
    free_impl(d);

    void *f = malloc_impl(600, "Test Allocation: 5");
    void *g = malloc_impl(150, "Test Allocation: 6");
    void *h = malloc_impl(44, "Test Allocation: 7");
    void *i = malloc_impl(3, "Test Allocation: 8");

    void *j = malloc_impl(6666, "Test Allocation: 9");
    void *k = malloc_impl(1192, "Test Allocation: 10");

    print_memory();

    return 0;
}

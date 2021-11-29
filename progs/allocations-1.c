#include <stdlib.h>
#include "../../allocator.c"

int main(void)
{
    void *a = malloc_impl(100, "Test Allocation: 0");
    void *b = malloc_impl(10,  "Test Allocation: 1"); /* Will be deleted */
    void *c = malloc_impl(100, "Test Allocation: 2");
    void *d = malloc_impl(100, "Test Allocation: 3"); /* Will be deleted */
    void *e = malloc_impl(100, "Test Allocation: 4");
    void *f = malloc_impl(100, "Test Allocation: 5");

    free_impl(b);
    free_impl(d);

    /* This will split:
     * - d with first fit (most recently freed)
     * - b with best fit
     * - f with worst fit
     */
    void *g = malloc_impl(10, "Test Allocation: 6");

    print_memory();

    return 0;
}

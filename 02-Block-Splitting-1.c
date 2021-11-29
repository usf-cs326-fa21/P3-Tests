#if 0
    source "${TEST_DIR}/lib/crunner" -lallocator
#endif

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <allocator.h>

size_t align(size_t orig_sz, size_t alignment)
{
    size_t new_sz = (orig_sz / alignment) * alignment;
    if (orig_sz % alignment != 0) {
        new_sz += alignment;
    }
    return new_sz;
}

test_start("Tests block splitting functionality");

subtest("Splitting a non-free block",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->free = false;

    struct mem_block *result = split_block(block, 1024);
    test_assert(result == NULL);

    free(block);
});

subtest("Invalid split sizes",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->free = true;
    block->size = 72;

    struct mem_block *result = split_block(block, 0);
    test_assert(result == NULL);

    result = split_block(block, sizeof(struct mem_block));
    test_assert(result == NULL);

    result = split_block(
            block,
            align(sizeof(struct mem_block) + 1, 8));
    test_assert(result == NULL);

    free(block);
});


subtest("Block is too small to be split",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->free = true;

    size_t split_sz = align(sizeof(struct mem_block) + 1, 8);

    block->size = 0;
    struct mem_block *result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = align(sizeof(struct mem_block) + 1, 8);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = align(sizeof(struct mem_block) + 2, 8);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    free(block);
});

subtest("Block doesn't have enough free space",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->free = true;

    block->size = 8000;
    size_t split_sz = align(sizeof(struct mem_block) + 7984, 8);
    struct mem_block *result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = 128;
    split_sz = align(sizeof(struct mem_block) + 3, 8);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = 136;
    split_sz = align(sizeof(struct mem_block) + 3, 8);
    result = split_block(block, split_sz);
    test_assert(result == NULL);

    block->size = 143;
    split_sz = align(sizeof(struct mem_block) + 3, 8);
    result = split_block(block, split_sz);
    test_assert(result == NULL);


    free(block);
});

subtest("Splitting splittable blocks! (Part 1)",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->block_number = 42;
    block->next_block = (void *) 42;
    block->free = true;
    block->size = 8000;

    size_t split_sz = align(sizeof(struct mem_block) + 672, 8);

    struct mem_block *result = split_block(block, split_sz);
    test_assert(result != NULL);

    printf(">>> We have split the block. Now let's check block properties:\n\n");
    /*
     * Things to check:
     *  - Both blocks are free when finished
     *  - The linked list is updated
     *  - Sizes are correct
     *  - The addresses are correct (relative to each other)
     *
     *  NOTE: We don't check the free list here. It might be reasonable to not
     *  update the free list during a split since most of the time one of the
     *  split blocks is about to be used.
     */

    test_assert(block->free == true);
    test_assert(result->free == true);

    test_assert(block->next_block == result);
    test_assert(result->next_block == (void *) 42);
    test_assert(result->prev_block == block);

    test_assert(block->size == 7264);
    test_assert(result->size == 736);

    test_assert((void *) result - (void *) block == 7264);

    free(block);
});

subtest("Splitting splittable blocks! (Part 2)",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->block_number = 1;
    block->next_block = (void *) 420;
    block->free = true;
    block->size = 144;

    size_t split_sz = align(sizeof(struct mem_block) + 1, 8);

    struct mem_block *result = split_block(block, split_sz);
    test_assert(result != NULL);

    printf(">>> We have split the block. Now let's check block properties:\n\n");

    test_assert(block->free == true);
    test_assert(block->next_block == result);

    test_assert(result->next_block == (void *) 420);
    test_assert(result->prev_block == block);
    test_assert(result->free == true);

    test_assert(block->size == 72);
    test_assert(result->size == 72);

    test_assert((void *) result - (void *) block == 72);

    free(block);
});

subtest("Splitting splittable blocks! (Part 3)",
{
    struct mem_block *block = malloc(sizeof(struct mem_block));
    block->block_number = 1;
    block->next_block = (void *) 12;
    block->free = true;
    block->size = 8192;

    size_t split_sz = align(sizeof(struct mem_block) + 3, 8);

    struct mem_block *result = split_block(block, split_sz);
    test_assert(result != NULL);

    printf(">>> We have split the block. Now let's check block properties:\n\n");

    test_assert(block->free == true);
    test_assert(block->next_block == result);

    test_assert(result->next_block == (void *) 12);
    test_assert(result->prev_block == block);
    test_assert(result->free == true);

    test_assert(block->size == 8120);
    test_assert(result->size == 72);

    test_assert((void *) result - (void *) block == 8120);

    free(block);
});




test_end

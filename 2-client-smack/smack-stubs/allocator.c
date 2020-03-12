// Allocator without function pointers

#include <smack.h>
#include <stdint.h>
#include <stdlib.h>

struct aws_allocator {
    void *(*mem_acquire)(struct aws_allocator *allocator, size_t size);
    void (*mem_release)(struct aws_allocator *allocator, void *ptr);
    void *(*mem_realloc)(struct aws_allocator *allocator, void *oldptr, size_t oldsize, size_t newsize);
    void *(*mem_calloc)(struct aws_allocator *allocator, size_t num, size_t size);
    void *impl;
    int dummy;
};

static struct aws_allocator default_allocator = {
    .mem_acquire = NULL,
    .mem_release = NULL,
    .mem_realloc = NULL,
    .mem_calloc = NULL,
};

struct aws_allocator *aws_default_allocator(void) {
    return &default_allocator;
}

void *aws_mem_acquire(struct aws_allocator *allocator, size_t size) {
    //assert(allocator == &default_allocator);
    return malloc(size);
}

void aws_mem_release(struct aws_allocator *allocator, void *ptr) {
    //assert(allocator == &default_allocator);
    free(ptr);
}

void *aws_mem_calloc(struct aws_allocator *allocator, size_t num, size_t size) {
    //assert(allocator == &default_allocator);
    return calloc(num, size);
}

int aws_mem_realloc(struct aws_allocator *allocator, void **ptr, size_t oldsize, size_t newsize) {
    //assert(allocator == &default_allocator);
    *ptr = realloc(*ptr, newsize);
    return 0;
}

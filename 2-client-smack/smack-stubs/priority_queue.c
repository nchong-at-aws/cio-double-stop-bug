/*
 * Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <smack.h>
#include <aws/common/priority_queue.h>

#include <string.h>

#define PARENT_OF(index) (((index)&1) ? (index) >> 1 : (index) > 1 ? ((index)-2) >> 1 : 0)
#define LEFT_OF(index) (((index) << 1) + 1)
#define RIGHT_OF(index) (((index) << 1) + 2)

static void s_swap(struct aws_priority_queue *queue, size_t a, size_t b) {
}

/* Precondition: with the exception of the given root element, the container must be
 * in heap order */
static bool s_sift_down(struct aws_priority_queue *queue, size_t root) {
    bool did_move;
    return did_move;
}

/* Precondition: Elements prior to the specified index must be in heap order. */
static bool s_sift_up(struct aws_priority_queue *queue, size_t index) {
    bool did_move;
    return did_move;
}

/*
 * Precondition: With the exception of the given index, the heap condition holds for all elements.
 * In particular, the parent of the current index is a predecessor of all children of the current index.
 */
static void s_sift_either(struct aws_priority_queue *queue, size_t index) {
}

int aws_priority_queue_init_dynamic(
    struct aws_priority_queue *queue,
    struct aws_allocator *alloc,
    size_t default_size,
    size_t item_size,
    aws_priority_queue_compare_fn *pred) {
    return AWS_OP_SUCCESS;
}

void aws_priority_queue_init_static(
    struct aws_priority_queue *queue,
    void *heap,
    size_t item_count,
    size_t item_size,
    aws_priority_queue_compare_fn *pred) {
}

bool aws_priority_queue_backpointer_index_valid(const struct aws_priority_queue *const queue, size_t index) {
    return true;
}

bool aws_priority_queue_backpointers_valid_deep(const struct aws_priority_queue *const queue) {
    return true;
}

bool aws_priority_queue_backpointers_valid(const struct aws_priority_queue *const queue) {
    return true;
}

bool aws_priority_queue_is_valid(const struct aws_priority_queue *const queue) {
    return true;
}

void aws_priority_queue_clean_up(struct aws_priority_queue *queue) {
}

int aws_priority_queue_push(struct aws_priority_queue *queue, void *item) {
    int rval;
    return rval;
}

int aws_priority_queue_push_ref(
    struct aws_priority_queue *queue,
    void *item,
    struct aws_priority_queue_node *backpointer) {
    return AWS_OP_SUCCESS;
}

static int s_remove_node(struct aws_priority_queue *queue, void *item, size_t item_index) {
    AWS_PRECONDITION(aws_priority_queue_is_valid(queue));
    AWS_PRECONDITION(item && AWS_MEM_IS_WRITABLE(item, queue->container.item_size));
    return AWS_OP_SUCCESS;
}

int aws_priority_queue_remove(
    struct aws_priority_queue *queue,
    void *item,
    const struct aws_priority_queue_node *node) {
    return AWS_OP_SUCCESS;
}

int aws_priority_queue_pop(struct aws_priority_queue *queue, void *item) {
    return AWS_OP_SUCCESS;
}

int aws_priority_queue_top(const struct aws_priority_queue *queue, void **item) {
    return AWS_OP_ERR;
}

size_t aws_priority_queue_size(const struct aws_priority_queue *queue) {
    return nondet_size_t();
}

size_t aws_priority_queue_capacity(const struct aws_priority_queue *queue) {
    return nondet_size_t();
}

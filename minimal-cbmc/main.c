/*
 * Essence of the C-IO double-stop datarace
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#define assume(x) __VERIFIER_assume(x)

// Fake-up aws_atomic {{{
#if FIX
struct aws_atomic_var {
    void *value;
};
#define AWS_ATOMIC_VAR_PTRVAL(var) ((var)->value)
#define AWS_ATOMIC_VAR_INTVAL(var) (*(aws_atomic_impl_int_t *)(var))
void aws_atomic_store_ptr(volatile struct aws_atomic_var *var, void *p) {
    AWS_ATOMIC_VAR_PTRVAL(var) = p;
}
void *aws_atomic_load_ptr(volatile const struct aws_atomic_var *var) {
    return AWS_ATOMIC_VAR_PTRVAL(var);
}
bool aws_atomic_compare_exchange_ptr(volatile struct aws_atomic_var *var, void **expected, void *desired) {
//  Replace following:
//  return aws_atomic_compare_exchange_ptr_explicit(
//      var, expected, desired, aws_memory_order_seq_cst, aws_memory_order_seq_cst);
//  with faked-up atomic compare-exchange:
    __VERIFIER_atomic_begin();
    bool update_succeeded = false;
    void *val = aws_atomic_load_ptr(var);
    if (val == *expected) {
        aws_atomic_store_ptr(var, desired);
        update_succeeded = true;
    }
    __VERIFIER_atomic_end();
    return update_succeeded;
}
#endif
// }}}

// Fake-up aws_mutex {{{
struct aws_mutex {
  pthread_mutex_t mutex_handle;
};

#define AWS_MUTEX_INIT \
 { .mutex_handle = PTHREAD_MUTEX_INITIALIZER }

void aws_mutex_lock(struct aws_mutex *mutex) {
    int retcode = pthread_mutex_lock(&mutex->mutex_handle);
    assert(retcode == 0);
}

void aws_mutex_unlock(struct aws_mutex *mutex) {
    int retcode = pthread_mutex_unlock(&mutex->mutex_handle);
    assert(retcode == 0);
}
// }}}

// Fake-up aws_linked_list {{{
struct aws_linked_list_node {
    struct aws_linked_list_node *next;
    struct aws_linked_list_node *prev;
};

struct aws_linked_list {
    struct aws_linked_list_node head;
    struct aws_linked_list_node tail;
};

bool aws_linked_list_empty(const struct aws_linked_list *list) {
    return list->head.next == &list->tail;
}

void aws_linked_list_init(struct aws_linked_list *list) {
    list->head.next = &list->tail;
    list->head.prev = NULL;
    list->tail.prev = &list->head;
    list->tail.next = NULL;
}

void aws_linked_list_insert_before(
    struct aws_linked_list_node *before,
    struct aws_linked_list_node *to_add) {
    to_add->next = before;
    to_add->prev = before->prev;
    before->prev->next = to_add;
    before->prev = to_add;
}

void aws_linked_list_push_back(struct aws_linked_list *list, struct aws_linked_list_node *node) {
    aws_linked_list_insert_before(&list->tail, node);
}

bool aws_linked_list_node_next_is_valid(const struct aws_linked_list_node *node) {
    return node && node->next && node->next->prev == node;
}

bool aws_linked_list_is_valid_deep(const struct aws_linked_list *list) {
    if (!list) {
        return false;
    }
    /* This could go into an infinite loop for a circular list */
    const struct aws_linked_list_node *temp = &list->head;
    /* Head must reach tail by following next pointers */
    bool head_reaches_tail = false;
    /* By satisfying the above and that edges are bidirectional, we
     * also guarantee that tail reaches head by following prev
     * pointers */
    while (temp) {
        if (temp == &list->tail) {
            head_reaches_tail = true;
            break;
        } else if (!aws_linked_list_node_next_is_valid(temp)) {
            /* Next and prev pointers should connect the same nodes */
            return false;
        }
        temp = temp->next;
    }
    return head_reaches_tail;
}

bool aws_linked_list_is_valid(const struct aws_linked_list *list) {
    if (list && list->head.next && list->head.prev == NULL && list->tail.prev && list->tail.next == NULL) {
        return aws_linked_list_is_valid_deep(list);
    }
    return false;
}
// }}}

// Fake-up aws_task {{{
typedef enum aws_task_status {
    AWS_TASK_STATUS_RUN_READY,
    AWS_TASK_STATUS_CANCELED,
} aws_task_status;

struct aws_task;

typedef void(aws_task_fn)(struct aws_task *task, void *arg, enum aws_task_status);

struct aws_task {
    aws_task_fn *fn;
    void *arg;
    uint64_t timestamp;
    struct aws_linked_list_node node;
    const char *type_tag;
};

void aws_task_init(struct aws_task *task, aws_task_fn *fn, void *arg, const char *type_tag) {
        task->fn = NULL;
        task->arg = NULL;
        task->timestamp = 0;
        task->node.next = NULL;
        task->node.prev = NULL;
        task->type_tag = NULL;
    task->fn = fn;
    task->arg = arg;
    task->type_tag = type_tag;
}
// }}}

// Fake-up event loop and epoll implementation {{{
struct aws_event_loop {
    void *impl_data;
};

struct epoll_loop {
    struct aws_mutex task_pre_queue_mutex;
    struct aws_linked_list task_pre_queue;
    struct aws_task stop_task;
#ifdef FIX
    struct aws_atomic_var stop_task_ptr;
#endif
};

struct aws_event_loop *fake_event_loop_new() {
    // model aws_event_loop_new_default
    struct aws_event_loop *event_loop = malloc(sizeof(struct aws_event_loop));
    assume(event_loop != NULL);
    struct epoll_loop *epoll_loop = malloc(sizeof(struct epoll_loop));
    assume(epoll_loop != NULL);
    event_loop->impl_data = epoll_loop;
    aws_linked_list_init(&epoll_loop->task_pre_queue);
    epoll_loop->task_pre_queue_mutex = (struct aws_mutex)AWS_MUTEX_INIT;
    assert(aws_linked_list_empty(&epoll_loop->task_pre_queue));
#ifdef FIX
    aws_atomic_store_ptr(&epoll_loop->stop_task_ptr, NULL);
#endif
    return event_loop;
}

void *fake_event_loop_main(void *args) {
    struct aws_event_loop *event_loop = args;
    struct epoll_loop *epoll_loop = event_loop->impl_data;
    aws_mutex_lock(&epoll_loop->task_pre_queue_mutex);
    assert(aws_linked_list_is_valid(&epoll_loop->task_pre_queue)); //< DETECT BUG
    aws_mutex_unlock(&epoll_loop->task_pre_queue_mutex);
    return NULL;
}

void fake_event_loop_run(pthread_t *tid, struct aws_event_loop *event_loop) {
    int ret = pthread_create(tid, 0, fake_event_loop_main, event_loop);
    assert(ret == 0);
}

void s_stop_task(struct aws_task *task, void *args, enum aws_task_status status) {
}

int fake_event_loop_stop(struct aws_event_loop *event_loop) {
    struct epoll_loop *epoll_loop = event_loop->impl_data;
#ifdef FIX
    void *expected_ptr = NULL;
    bool update_succeeded =
        aws_atomic_compare_exchange_ptr(&epoll_loop->stop_task_ptr, &expected_ptr, &epoll_loop->stop_task);
    if (false == update_succeeded) {
        /* the stop task is already scheduled. */
        return 0;
    }
#endif
    aws_task_init(&epoll_loop->stop_task, s_stop_task, event_loop, "epoll_event_loop_stop"); //< BUG if task is already in list
    aws_mutex_lock(&epoll_loop->task_pre_queue_mutex);
    assert(aws_linked_list_is_valid(&epoll_loop->task_pre_queue));
    aws_linked_list_push_back(&epoll_loop->task_pre_queue, &epoll_loop->stop_task.node);
    aws_mutex_unlock(&epoll_loop->task_pre_queue_mutex);
    return 0;
}
// }}}

struct aws_event_loop *event_loop;
struct epoll_loop *epoll_loop;

int main() {
    pthread_t tid_event_loop;
    struct aws_event_loop *event_loop = fake_event_loop_new();
    fake_event_loop_run(&tid_event_loop, event_loop);
    fake_event_loop_stop(event_loop);
    fake_event_loop_stop(event_loop); //< 2nd call can corrupt list
    pthread_join(tid_event_loop, 0);
    return 0;
}

#include <smack.h>
#include <stddef.h>
typedef size_t aws_atomic_impl_int_t;
#include <aws/common/atomics.h>

size_t aws_atomic_load_int(volatile const struct aws_atomic_var *var) {
    return AWS_ATOMIC_VAR_INTVAL(var);
}

void aws_atomic_init_int(volatile struct aws_atomic_var *var, size_t n) {
    AWS_ATOMIC_VAR_INTVAL(var) = n;
}
static inline int aws_atomic_priv_xlate_order(enum aws_memory_order order) {
    switch (order) {
        case aws_memory_order_relaxed:
            return __ATOMIC_RELAXED;
        case aws_memory_order_acquire:
            return __ATOMIC_ACQUIRE;
        case aws_memory_order_release:
            return __ATOMIC_RELEASE;
        case aws_memory_order_acq_rel:
            return __ATOMIC_ACQ_REL;
        case aws_memory_order_seq_cst:
            return __ATOMIC_SEQ_CST;
        default: /* Unknown memory order */
            abort();
    }
}
#if 0
void aws_atomic_store_ptr_explicit(volatile struct aws_atomic_var *var, void *p, enum aws_memory_order memory_order) {
    __atomic_store_n(&AWS_ATOMIC_VAR_PTRVAL(var), p, aws_atomic_priv_xlate_order(memory_order));
}
void aws_atomic_store_ptr(volatile struct aws_atomic_var *var, void *p) {
    aws_atomic_store_ptr_explicit(var, p, aws_memory_order_seq_cst);
}
void *aws_atomic_load_ptr_explicit(volatile const struct aws_atomic_var *var, enum aws_memory_order memory_order) {
    return __atomic_load_n(&AWS_ATOMIC_VAR_PTRVAL(var), aws_atomic_priv_xlate_order(memory_order));
}
void *aws_atomic_load_ptr(volatile const struct aws_atomic_var *var) {
    return aws_atomic_load_ptr_explicit(var, aws_memory_order_seq_cst);
}
#endif
void aws_atomic_store_ptr(volatile struct aws_atomic_var *var, void *p) {
    AWS_ATOMIC_VAR_PTRVAL(var) = p;
}
void *aws_atomic_load_ptr(volatile const struct aws_atomic_var *var) {
    return AWS_ATOMIC_VAR_PTRVAL(var);
}
bool aws_atomic_compare_exchange_ptr(volatile struct aws_atomic_var *var, void **expected, void *desired) {
    static first = true;
    if (first) {
        first = false;
        return true;
    }
    return false;
//  __SMACK_code("call corral_atomic_begin();");
//  //__VERIFIER_atomic_begin();
//  bool update_succeeded = false;
//  void *val = aws_atomic_load_ptr(var);
//  if (val == *expected) {
//      aws_atomic_store_ptr(var, desired);
//      update_succeeded = true;
//  }
//  //__VERIFIER_atomic_end();
//  __SMACK_code("call corral_atomic_end();");
//  return update_succeeded;
}

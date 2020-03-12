#include <smack.h>
#include <stddef.h>
#include <aws/common/zero.h>

bool aws_is_mem_zeroed(const void *buf, size_t bufsize) {
    return true;
  //size_t x = nondet_size_t();
  //assume(0 <= x && x < bufsize);
  //return ((char *)buf)[x] == 0;
}

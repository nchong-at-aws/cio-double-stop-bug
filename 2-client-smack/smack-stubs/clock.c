#include <smack.h>
#include <stdint.h>

uint64_t nondet_uint64_t();
int aws_high_res_clock_get_ticks(uint64_t *timestamp) {
    *timestamp = nondet_uint64_t();
    return 0;
}

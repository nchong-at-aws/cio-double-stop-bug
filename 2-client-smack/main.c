/*
 * Client calls into library
 */

#include <smack.h>
#include <pthread.h>
#include <stdlib.h>
#include <aws/common/clock.h>
#include <aws/common/allocator.h>
#include <aws/io/event_loop.h>

int main() {
    // Create new event loop object
    struct aws_event_loop *event_loop = aws_event_loop_new_default(aws_default_allocator(), aws_high_res_clock_get_ticks);
    if (!event_loop) return 1;

    // Create new event loop thread
    int ret = aws_event_loop_run(event_loop);
    if (ret != 0) return 1;

    assert(!aws_event_loop_thread_is_callers_thread(event_loop));

    // Try to cause double-stop datarace
    aws_event_loop_stop(event_loop);
    aws_event_loop_stop(event_loop);
    return 0;
}

SMACK=smack
TIME_LIMIT ?= 0
UNROLL ?= 2
CTXBOUND ?= 2
SMACK_OPTIONS = --pthread \
	--time-limit $(TIME_LIMIT) \
	--unroll $(UNROLL) \
	--context-bound $(CTXBOUND) \
	# empty last line
SOURCE_FILES = main.c \
    aws-c-common/source/posix/thread.c \
	aws-c-common/source/posix/mutex.c \
	aws-c-common/source/task_scheduler.c \
	aws-c-common/source/array_list.c \
	aws-c-io/source/linux/epoll_event_loop.c \
	aws-c-io/source/event_loop.c \
    smack-stubs/linked_list.c \
    smack-stubs/epoll.c \
    smack-stubs/priority_queue.c \
	smack-stubs/allocator.c \
	smack-stubs/clock.c \
	smack-stubs/atomics.c \
	smack-stubs/zero.c \
	# empty last line
CLANG_OPTIONS = \
    -I aws-c-common/include \
    -I aws-c-io/include \
    -I include \
	-DAWS_STATIC_LOG_LEVEL=0 \
    -DAWS_NO_STATIC_IMPL \
	-DSMACK \
	-DDISABLE_PTHREAD_ASSERTS \
	# empty last line

all: bug fix

.PHONY: bug
bug: $(SOURCE_FILES)
	-rm -f bug.txt
	@date | tee -a bug.txt
	-time $(SMACK) $(SMACK_OPTIONS) --clang-options "$(CLANG_OPTIONS)" $^ 2>&1 | tee -a bug.txt
	@date | tee -a bug.txt

.PHONY: fix
fix: $(SOURCE_FILES)
	-rm -f fix.txt
	@date | tee -a fix.txt
	-time $(SMACK) $(SMACK_OPTIONS) --clang-options "$(CLANG_OPTIONS) -DFIX" $^ 2>&1 | tee -a fix.txt
	@date | tee -a fix.txt

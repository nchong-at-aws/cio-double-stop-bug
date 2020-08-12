TOOL=esbmc
TIME_LIMIT ?= 0
UNROLL ?= 2
CTXBOUND ?= 2
TOOL_OPTIONS = \
	--timeout $(TIME_LIMIT) \
	--unwind $(UNROLL) \
	--context-bound $(CTXBOUND) \
	--no-unwinding-assertions \
	# empty last line
SOURCE_FILES = main.c \
	# empty last line
CLANG_OPTIONS = \
	# empty last line

all: bug fix

.PHONY: bug
bug: $(SOURCE_FILES)
	-rm -f bug.txt
	@date | tee -a bug.txt
	-time $(TOOL) $(TOOL_OPTIONS) $^ 2>&1 | tee -a bug.txt
	@date | tee -a bug.txt

.PHONY: fix
fix: $(SOURCE_FILES)
	-rm -f fix.txt
	@date | tee -a fix.txt
	-time $(TOOL) $(TOOL_OPTIONS) -DFIX $^ 2>&1 | tee -a fix.txt
	@date | tee -a fix.txt

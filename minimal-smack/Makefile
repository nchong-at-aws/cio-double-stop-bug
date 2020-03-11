SMACK=smack
TIME_LIMIT ?= 0
UNROLL ?= 2
CTXBOUND ?= 2
SMACK_OPTIONS = --pthread \
	--time-limit $(TIME_LIMIT) \
	--unroll $(UNROLL) \
	--context-bound $(CTXBOUND) \
	--warn info \
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
	-time $(SMACK) $(SMACK_OPTIONS) --clang-options "$(CLANG_OPTIONS)" $^ 2>&1 | tee -a bug.txt
	@date | tee -a bug.txt

.PHONY: fix
fix: $(SOURCE_FILES)
	-rm -f fix.txt
	@date | tee -a fix.txt
	-time $(SMACK) $(SMACK_OPTIONS) --clang-options "$(CLANG_OPTIONS) -DFIX" $^ 2>&1 | tee -a fix.txt
	@date | tee -a fix.txt

To find the bug:
time smack --pthread --unroll 2 --context-bound 2 --clang-options="" main-smack.c

To verify the fix:
time smack --pthread --unroll 2 --context-bound 2 --clang-options="-DFIX" main-smack.c

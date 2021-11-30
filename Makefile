memperf:
gcc -Wall -Werror -fsanitize=address -o memperf memperf.c mymalloc.h mymalloc.c

CFLAGS=-Wall -Wextra -std=c11 -pedantic

heap: alloc.c
	$(CC) $(CFLAGS) -o heap alloc.c

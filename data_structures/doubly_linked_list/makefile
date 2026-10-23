CFLAGS = -g -std=gnu99 -Wno-error=implicit-function-declaration -Wno-error=implicit-int
PROGS = doubly_linked_list

all: $(PROGS)

doubly_linked_list: main.c
	$(CC) $(CFLAGS) -o $@ main.c

clean:
	rm -f $(PROGS) *~

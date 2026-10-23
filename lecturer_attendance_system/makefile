CFLAGS = -g -std=gnu99 -Wno-error=implicit-function-declaration -Wno-error=implicit-int
PROGS = lecturer_attendance_system

all: $(PROGS)

lecturer_attendance_system: main.c
	$(CC) $(CFLAGS) -o $@ main.c

clean:
	rm -f $(PROGS) *~

CFLAGS = -g $(shell mysql_config --cflags)
LDLIBS = $(shell mysql_config --libs)
PROGS = myclient serverconnectedtosql

all: $(PROGS)

clean:
	rm -f $(PROGS) *~

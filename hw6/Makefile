CC      = clang
CFLAGS  += -D_GNU_SOURCE -gdwarf-4 -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS += -gdwarf-4 -O2 -std=c11

all: groups table-test

groups: groups.o array-list.o linked-list.o map.o table.o hash.o dict.o
	$(CC) $(LDFLAGS) -o $@ $^

table-test: table.o array-list.o linked-list.o table-test.o tests.o hash.o
	$(CC) $(LDFLAGS) -o $@ $^

export LSAN_OPTIONS := suppressions=memcheck.supp,print_suppressions=0
export ASAN_OPTIONS := detect_leaks=1
export MallocNanoZone := 0
memcheck-test: table.c array-list.c linked-list.c table-test.c tests.c hash.c
	$(CC) $(CFLAGS) -fsanitize=address -o table-test-mem $^
	-./table-test-mem
	rm -rf table-test-mem table-test-mem.dSYM

memcheck-groups: groups.c array-list.c linked-list.c map.c table.c hash.c dict.c
	$(CC) $(CFLAGS) -fsanitize=address -o groups-test-mem $^
	-./groups-test-mem -t tests/tiny.txt
	-./groups-test-mem -m tests/tiny.txt
	rm -rf groups-test-mem groups-test-mem.dSYM
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o groups table-test *.dSYM


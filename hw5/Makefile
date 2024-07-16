CC      = clang
CFLAGS  += -gdwarf-4 -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS += -gdwarf-4 -O2 -std=c11

all: groups map-test

groups: groups.o array-list.o linked-list.o map.o
	$(CC) $(LDFLAGS) -o $@ $^

map-test: map.o array-list.o linked-list.o map-test.o tests.o
	$(CC) $(LDFLAGS) -o $@ $^

export LSAN_OPTIONS := suppressions=memcheck.supp,print_suppressions=0
export ASAN_OPTIONS := detect_leaks=1
export MallocNanoZone := 0

memcheck-test:  map.c array-list.c linked-list.c map-test.c tests.c
	$(CC) $(CFLAGS) -fsanitize=address -o map-test-mem $^
	-./map-test-mem
	rm -rf map-test-mem map-test-mem.dSYM

memcheck-groups: groups.c array-list.c linked-list.c map.c
	$(CC) $(CFLAGS) -fsanitize=address -o groups-mem $^
	-./groups-mem
	rm -rf groups-mem groups-mem.dSYM

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o groups map-test *.dSYM


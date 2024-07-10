CC      = clang
CFLAGS  += -gdwarf-4 -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS += -gdwarf-4 -O2 -std=c11

linked-list-test: tests.o linked-list.o linked-list-test.o
	$(CC) $(LDFLAGS) -o $@ $^

export LSAN_OPTIONS := suppressions=memcheck.supp,print_suppressions=0
export ASAN_OPTIONS := detect_leaks=1
export MallocNanoZone := 0
memcheck: tests.c linked-list.c linked-list-test.c
	$(CC) $(CFLAGS) -fsanitize=address -o linked-list-test-mem $^
	-./linked-list-test-mem
	rm -rf linked-list-test-mem linked-list-test-mem.dSYM

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o linked-list-test *.dSYM


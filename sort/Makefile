CC      = clang
CFLAGS  += -gdwarf-4 -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS += -gdwarf-4 -O2 -std=c11

sort-test: sort-test.o sort.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o sort-test *.dSYM


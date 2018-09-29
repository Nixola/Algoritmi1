CC=gcc
CFLAGS=--std=c11 -Wall -Wextra -Wpedantic -Werror

debug: CFLAGS += -g
debug: main.o
	$(CC) $(CFLAGS) -o Dmain main.o

release: CFLAGS += -O3
release: main.o
	$(CC) $(CFLAGS) -o main main.o

benchmark: CFLAGS += -Dbenchmark
benchmark: release

clean:
	rm -f main Dmain *.o
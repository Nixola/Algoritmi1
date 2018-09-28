CC=gcc
CFLAGS=--std=c11 -Wall -Wextra -Wpedantic -Werror -g

hellomake: main.o
	$(CC) -o main main.o
# Makefile for the LI2 project
CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wundef -Wformat=2 -pedantic -O1 -g -fno-omit-frame-pointer -std=c11 -fsanitize=address -fsanitize=undefined
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
SRC = src
BIN = bin
OBJS = $(SRC)/main.o $(SRC)/jogo.o
TEST_OBJS = $(SRC)/testar.o $(SRC)/jogo.o

.PHONY: all clean

all: clean jogo  # Clean before building main executable

jogo: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o jogo $(OBJS)

testar: clean  # Clean before building/running tests
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c -o $(SRC)/testar.o $(SRC)/testar.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c -o $(SRC)/jogo.o $(SRC)/jogo.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(LDFLAGS) -lcunit -o testar $(TEST_OBJS)
	./testar
	gcov -o $(SRC) $(SRC)/jogo.c $(SRC)/testar.c

clean:
	rm -f jogo testar $(SRC)/*.o $(SRC)/*.gcda $(SRC)/*.gcno *.gcov
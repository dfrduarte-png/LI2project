CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wundef -Wformat=2 -pedantic -O1 -g -fno-omit-frame-pointer -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
SRC = src
BIN = bin
OBJS = $(SRC)/main.o $(SRC)/jogo.o
TEST_OBJS = $(SRC)/testar.o $(SRC)/jogo.o

all: jogo

jogo: $(OBJS)
	$(CC) $(CFLAGS) -o jogo $(OBJS)

testar: CFLAGS += $(GCOV_FLAGS)
testar: $(TEST_OBJS)
	$(CC) $(CFLAGS) -lcunit -o testar $(TEST_OBJS)
    ./testar
    gcov $(SRC)/*.c

clean:
    rm -f *.o jogo testar *.gcda *.gcno *.gcov
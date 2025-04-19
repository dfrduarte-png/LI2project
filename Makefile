CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wundef -Wformat=2 -pedantic -O1 -g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -std=c11
SRC = src
BIN = bin
OBJS = $(SRC)/main.o $(SRC)/jogo.o
TEST_OBJS = $(SRC)/testar.o $(SRC)/jogo.o

all: jogo

jogo: $(OBJS)
	$(CC) $(CFLAGS) -o jogo $(OBJS)

testar: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o testar $(TEST_OBJS)

clean:
	rm -f *.o jogo testar
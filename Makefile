CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

TARGET = mini_shell
SRC = src/main.c src/parser.c src/executor.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

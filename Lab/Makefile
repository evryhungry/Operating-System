# Makefile for wspipe

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = wspipe
SRC = wspipe.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
TARGET = main

SRCS = main.c bellman_ford.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c bellman_ford.h
	$(CC) $(CFLAGS) -c main.c

bellman_ford.o: bellman_ford.c bellman_ford.h
	$(CC) $(CFLAGS) -c bellman_ford.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean

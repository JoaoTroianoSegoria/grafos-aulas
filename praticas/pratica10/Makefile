CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
TARGET = main

SRCS = main.c hamiltoniano.c tps.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c hamiltoniano.h tps.h
	$(CC) $(CFLAGS) -c main.c

hamiltoniano.o: hamiltoniano.c hamiltoniano.h
	$(CC) $(CFLAGS) -c hamiltoniano.c

tps.o: tps.c tps.h
	$(CC) $(CFLAGS) -c tps.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean

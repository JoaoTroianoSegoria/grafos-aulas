CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2
TARGET = main

SRCS = main.c coloracao.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c coloracao.h
	$(CC) $(CFLAGS) -c main.c

coloracao.o: coloracao.c coloracao.h
	$(CC) $(CFLAGS) -c coloracao.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

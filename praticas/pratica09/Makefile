CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
TARGET = main

SRCS = main.c cobertura.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c cobertura.h
	$(CC) $(CFLAGS) -c main.c

cobertura.o: cobertura.c cobertura.h
	$(CC) $(CFLAGS) -c cobertura.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean

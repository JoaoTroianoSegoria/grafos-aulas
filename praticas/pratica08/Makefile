CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
TARGET = main

SRCS = main.c agm.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c agm.h
	$(CC) $(CFLAGS) -c main.c

agm.o: agm.c agm.h
	$(CC) $(CFLAGS) -c agm.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean

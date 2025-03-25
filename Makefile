CC = gcc
CFLAGS = -Wall -Ifinal

SRCS = $(wildcard final/*.c)

OBJS = $(SRCS:.c=.o)

TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

final/%.o: final/%.c final/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f final/*.o $(TARGET)

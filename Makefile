# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Ipart_a

# Source files
SRCS = $(wildcard part_a/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = a.out

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

# Compile source files to object files
part_a/%.o: part_a/%.c part_a/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f part_a/*.o $(TARGET)

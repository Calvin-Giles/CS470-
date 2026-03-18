# define the C compiler to use
CC = gcc

# define any compile-time warning messages
CFLAGS = -Wall -g

# define target executable name
TARGET = CalvinFS

# list all source files
SRCS = testfile.c CalvinFS.c

# 'all' is the default target
all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# remove compiled executable
clean:
	rm -f $(TARGET)

# run the program
run: all
	./$(TARGET)
#define the C compiler to use
CC=gcc

#define any compile-time warning messages
CFLAGS=-Wall -g

#define target file exe name
TARGET=ProcessManagment

#'all' is the default target
all: $(TARGET)

#the target will be rebuilt when 'make' is run
$(TARGET): $(TARGET).c
		$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
# compiler: gcc / clang
CC = gcc

# compiler flags:
CFLAGS = -Wall

# target: mywc
TARGET = mywc

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) : $(TARGET)

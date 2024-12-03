CC = gcc
CFLAGS = -Wall -g

# 目标可执行文件
TARGET = huffmanT
TEST_TARGET = test_huffmanT

# 源文件
SRCS = huffmanT.c main.c
TEST_SRCS = huffmanT.c test_huffmanT.c

# 对象文件
OBJS = huffmanT.o main.o
TEST_OBJS = huffmanT.o test_huffmanT.o

.PHONY: all clean test

all: $(TARGET)

$(TARGET): main.o huffmanT.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o huffmanT.o

test: $(TEST_TARGET)

$(TEST_TARGET): test_huffmanT.o huffmanT.o
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_huffmanT.o huffmanT.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)
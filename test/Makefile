CC = gcc
CFLAGS = -Wall -g
9CC = ../9cc

SRCS = test.c help.c util.c
OBJS = $(SRCS:.c=.o)
TARGET = test

check: $(TARGET)
	./$(TARGET) && echo "OK"
clean:
	rm -f $(TARGET) *.s *.o a.out core *~
format:
	clang-format -i $(SRCS)
$(TARGET): $(OBJS)
	cc -o $@ $^

util.o: util.c
	$(CC) $(CFLAGS) -c $<

test.o: test.c
	$(9CC) $< >test.s
	$(CC) -o test.o -c test.s

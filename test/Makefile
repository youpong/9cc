# *.9c: 9cc source
# *.c:  c souce

CC = gcc
CFLAGS = -Wall -g
9CC = ../9cc

SRCS = help.c util.c print_result.c
9CC_SRCS = test.9c test_unary_minus.9c test_pointer.9c test_sizeof.9c \
	   test_array.9c

OBJS = $(SRCS:.c=.o)
9CC_OBJS = $(9CC_SRCS:.9c=.o)

TARGET = test

check: $(TARGET)
	./$(TARGET) 
clean:
	rm -f $(TARGET) *.s *.o a.out core *~
format:
	clang-format -i $(SRCS) $(9CC_SRCS)

$(TARGET): $(OBJS) $(9CC_OBJS)
	$(CC) -o $@ $^

.SUFFIXES: .9c .s
.SUFFIXES: .s .o

.9c.s:
	$(9CC) $< >$@
.s.o:
	$(CC) -c $<
.c.o:
	$(CC) $(CFLAGS) -c $<


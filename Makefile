CC = gcc
CFLAGS = -Wall -std=c11 -g --coverage
LIBS = -lgcov

TARGET = 9cc
SRCS = main.c init.c  symbol.c tokenize.c  parse.c codegen.c \
       node.c debug.c util.c   util_test.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
clean:
	rm -f $(TARGET) node_test a.out\
	      $(OBJS) tmp* *.png
format:
	clang-format -i $(SRCS) *.h
gcov:
	gcov $(SRCS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

$(OBJS): 9cc.h util.h

test: $(TARGET)
	./$(TARGET) -test
	./test.sh

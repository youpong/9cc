CC = clang
CFLAGS=-Wall -std=c11 -g

SRCS=codegen.c main.c node.c parse.c tokenize.c util.c symbol.c \
     init.c util_test.c debug.c 
OBJS=$(SRCS:.c=.o)

all: 9cc
clean:
	rm -f 9cc codegen_test node_test a.out\
	      $(OBJS) tmp* *.png
format:
	clang-format -i $(SRCS) *.h

9cc: $(OBJS)
	$(CC) -o $@ $^

$(OBJS): 9cc.h util.h

test: 9cc codegen_test node_test 
	./9cc -test
	./test.sh

codegen_test: codegen_test.o node.o util.o symbol.o
codegen_test.o: codegen.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

node_test: node_test.o util.o
node_test.o: node.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

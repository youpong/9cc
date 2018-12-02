CC = clang
CFLAGS=-Wall -std=c11 -g

SRCS=codegen.c main.c node.c parse.c tokenize.c util.c util_test.c
OBJS=$(SRCS:.c=.o)

all: 9cc
9cc: $(OBJS)
	$(CC) -o $@ $^

$(OBJS): 9cc.h util.h

test: 9cc codegen_test node_test parse_test
	./9cc -test
	./test.sh

codegen_test: codegen_test.o node.o util.o
codegen_test.o: codegen.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

node_test: node_test.o util.o
node_test.o: node.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

parse_test: parse_test.o node.o tokenize.o util.o
parse_test.o: parse.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

clean:
	rm -f 9cc codegen_test node_test parse_test \
	      a.out *.o *~ tmp*




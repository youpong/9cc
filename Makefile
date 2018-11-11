CFLAGS=-Wall -std=c11 -g

SRCS=main.c node.c tokenize.c parse.c codegen.c util.c
OBJS=$(SRCS:.c=.o)

all: 9cc
#9cc: main.o node.o tokenize.o parse.o codegen.o util.o
9cc: $(OBJS)
	$(CC) -o $@ $^

$(OBJS): 9cc.h

test: 9cc codegen_test node_test parse_test
	./test.sh

codegen_test: codegen_test.o node.o
codegen_test.o: codegen.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

node_test: node_test.o
node_test.o: node.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

parse_test: parse_test.o node.o tokenize.o
parse_test.o: parse.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

clean:
	rm -f 9cc codegen_test node_test parse_test \
	      a.out *.o *~ tmp*




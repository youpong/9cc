CFLAGS=-g

all: 9cc
9cc: 9cc.o node.o parse.o gen.o util.o
9cc.o: 9cc.c 9cc.h
node.o: node.c 9cc.h
parse.o: parse.c 9cc.h
gen.o: gen.c 9cc.h

test: 9cc gen_test node_test
	./test.sh

gen_test: gen_test.o node.o
gen_test.o: gen.c 9cc.h
	$(CC) -c -D UNIT_TEST -o gen_test.o gen.c

node_test: node_test.o
node_test.o: node.c 9cc.h
	$(CC) -c -D UNIT_TEST -o node_test.o node.c

clean:
	rm -f 9cc gen_test node_test a.out *.o *~ tmp*




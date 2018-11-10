CFLAGS=-g

all: 9cc
9cc: main.o node.o tokenize.o parse.o gen.o util.o
	$(CC) -o $@ $^
main.o:     main.c     9cc.h
node.o:     node.c     9cc.h
parse.o:    parse.c    9cc.h
tokenize.o: tokenize.c 9cc.h
gen.o:      gen.c      9cc.h

test: 9cc gen_test node_test parse_test
	./test.sh

gen_test: gen_test.o node.o
gen_test.o: gen.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

node_test: node_test.o
node_test.o: node.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

parse_test: parse_test.o node.o tokenize.o
parse_test.o: parse.c 9cc.h
	$(CC) $(CFLAGS) -c -D UNIT_TEST -o $@ $<

clean:
	rm -f 9cc gen_test node_test parse_test a.out *.o *~ tmp*




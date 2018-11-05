all: 9cc
9cc: 9cc.o parse.o gen.o
9cc.o: 9cc.c 9cc.h
parse.o: parse.c 9cc.h
gen.o: gen.c 9cc.h

test: 9cc
	./test.sh
gen_test: gen_test.o parse.o
gen_test.o: gen.c 9cc.h
	$(CC) -c -D UNIT_TEST -o gen_test.o gen.c

clean:
	rm -f 9cc gen_test *.o *~ tmp*

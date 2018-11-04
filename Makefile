9cc: 9cc.o parse.o gen.o
9cc.o: 9cc.c 9cc.h
parse.o: parse.c 9cc.h
gen.o: gen.c 9cc.h

test: 9cc
	./test.sh
clean:
	rm -f 9cc *.o *~ tmp*




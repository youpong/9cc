CC = gcc

clean:
	rm -f a.out core *.o *.class fibonacci.s

fibonacci: fib_main.o fibonacci.o
fibonacci.o: fibonacci.s
	$(CC) -c $<
fibonacci.s: fibonacci.c
	../9cc $< > $@

Fibonacci.class: Fibonacci.java
	javac $<

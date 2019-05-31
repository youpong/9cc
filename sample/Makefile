CC = gcc
TARGET = fibonacci
SRCS = fibonacci.c fib_main.c Fibonacci.java

all: $(TARGET)
clean:
	rm -f $(TARGET) a.out core *.o *.class fibonacci.s
format:
	clang-format -i $(SRCS)
$(TARGET): fib_main.o fibonacci.o
fibonacci.o: fibonacci.s
	$(CC) -c $<
fibonacci.s: fibonacci.c
	../9cc $< > $@

Fibonacci.class: Fibonacci.java
	javac $<

CC = gcc
CFLAGS = -Wall -g
9CC = ../9cc

SRC = 
TARGET = test

check: $(TARGET)
	./$(TARGET) && echo "OK"
clean:
	rm -f *.s *.o a.out core $(TARGET)

$(TARGET): test.o util.o
	cc -o $@ $^

util.o: util.c
	$(CC) $(CFLAGS) -c $<

test.o: test.c
	$(9CC) $< >test.s
	$(CC) -o test.o -c test.s

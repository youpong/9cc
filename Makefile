CC = gcc
#CFLAGS = -Wall -std=c11 -g -D_POSIX_C_SOURCE=200809L --coverage
CFLAGS = -Wall -std=c11 -g -D_POSIX_C_SOURCE=200809L
# electrice fence
#LIBS = -lgcov -lefence
LIBS = -lgcov 

TARGET = 9cc
SRCS = main.c init.c  symbol.c tokenize.c  parse.c codegen.c \
       node.c debug.c util.c   util_test.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
clean:
	rm -f $(TARGET) $(OBJS) tmp* *~ \
              TAGS a.out core *.gcov *.gcno *.gcda *.png
	cd test; make clean
	cd sample; make clean
check: $(TARGET)
	./$(TARGET) -test
	cd test; make clean check
format:
	clang-format -i $(SRCS) *.h
	cd test; make format
	cd sample; make format
gcov: $(TARGET)
	gcov $(SRCS)
tags:
	etags $(SRCS) *.h

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

$(OBJS): 9cc.h util.h


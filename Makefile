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
	rm -f $(TARGET) $(OBJS) a.out tmp* \
               *.gcov *.gcno *.gcda *.png
test: $(TARGET)
	./$(TARGET) -test
	./test.sh
format:
	clang-format -i $(SRCS) *.h
gcov: $(TARGET)
	gcov $(SRCS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

$(OBJS): 9cc.h util.h


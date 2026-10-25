CFLAGS=-std=c11 -g -static
SRCS=9cc.c codegen.c parse.c
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	cc $(CFLAGS) -o $@ $(OBJS)

test: 9cc 
	./test.sh

clean:
	rm -f 9cc $(OBJS) *~ tmp*

.PHONY: test clean

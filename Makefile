CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	cc $(CFLAGS) -o $@ $(OBJS)

$(OBJS): 9cc.h

test: 9cc 
	./test.sh

clean:
	rm -f 9cc $(OBJS) *~ tmp*

.PHONY: test clean

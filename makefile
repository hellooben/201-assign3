OBJS = integer.o real.o string.o sll.o dll.o queue.o stack.o vertex.o edge.o dll-*-*.o binomial-*-*.o binomial.o
OPTS = -Wall -Wextra -std=c99
EXECS = test-dll test-binomial
EXTRAS = integer.c real.c string.c queue.c sll.c stack.c vertex.c edge.c
OEXTRAS = integer.o real.o string.o queue.o sll.o stack.o vertex.o edge.o
# EXTRAS = integer.c real.c string.c sll.c dll.c stack.c queue.c
# OEXTRAS = integer.o real.o string.o sll.o dll.o stack.o queue.o

all : test-dll test-binomail

# test-sll :
# 	gcc $(OPTS) -c sll.c integer.c sll-0-0.c
# 	gcc $(OPTS) sll-0-0.o integer.o sll.o -o test-sll

test-dll :
	gcc $(OPTS) -c dll.c integer.c dll-1-0.c
	gcc $(OPTS) dll-1-0.o integer.o dll.o -o test-dll

test-binomial :
	gcc $(OPTS) -c binomial.c dll.c $(EXTRAS) binomial-2-3.c
	gcc $(OPTS) binomial-2-3.o $(OEXTRAS) dll.o binomial.o -o test-binomial

valgrind :
	echo testing doubly-linked list
	valgrind ./test-dll
	echo
	echo testing binomial
	valgrind ./test-binomial
	echo

test :
	./test-dll
	./test-binomial

clean :
	rm -f $(OBJS) $(EXECS)

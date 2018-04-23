OBJS = integer.o real.o string.o sll.o dll.o dll-*-*.o
OPTS = -Wall -Wextra -std=c99
EXECS = test-dll
# EXTRAS = integer.c real.c string.c sll.c dll.c stack.c queue.c
# OEXTRAS = integer.o real.o string.o sll.o dll.o stack.o queue.o

all : test-dll 

# test-sll :
# 	gcc $(OPTS) -c sll.c integer.c sll-0-0.c
# 	gcc $(OPTS) sll-0-0.o integer.o sll.o -o test-sll

test-dll :
	gcc $(OPTS) -c dll.c integer.c dll-1-0.c
	gcc $(OPTS) dll-1-0.o integer.o dll.o -o test-dll

valgrind :
	echo testing doubly-linked list
	valgrind ./test-dll
	echo

test :
	./test-dll

clean :
	rm -f $(OBJS) $(EXECS)

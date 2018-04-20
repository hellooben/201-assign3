OBJS = integer.o real.o string.o sll.o dll.o stack.o queue.o heap.o a.out bst.o gst.o avl.o trees.o scanner.o sll-*-*.o dll-*-*.o stack-*-*.o queue-*-*.o gst-*-*.o avl-*-*.o
LOPTS = -Wall -Wextra -std=c99
EXTRAS = integer.c real.c string.c sll.c dll.c stack.c queue.c
OEXTRAS = integer.o real.o string.o sll.o dll.o stack.o queue.o

all : test-sll test-dll test-stack test-queue test-bst test-gst test-avl trees

test-sll :
	gcc $(LOPTS) -c sll.c integer.c sll-0-0.c
	gcc $(LOPTS) sll-0-0.o integer.o sll.o -o test-sll

test-dll :
	gcc $(LOPTS) -c dll.c integer.c dll-0-0.c
	gcc $(LOPTS) dll-0-0.o integer.o dll.o -o test-dll

test-stack :
	gcc $(LOPTS) -c stack.c dll.c integer.c stack-0-0.c
	gcc $(LOPTS) stack-0-0.o integer.o dll.o stack.o -o test-stack

test-queue :
	gcc $(LOPTS) -c queue.c sll.c integer.c queue-0-0.c
	gcc $(LOPTS) queue-0-0.o integer.o sll.o queue.o -o test-queue

test-bst :
	gcc $(LOPTS) -c bst.c $(EXTRAS) bst-2-0.c
	gcc $(LOPTS) bst-2-0.o $(OEXTRAS) bst.o -o test-bst

test-gst :
	gcc $(LOPTS) -c gst.c bst.c $(EXTRAS) gst-0-0.c
	gcc $(LOPTS) gst-0-0.o $(OEXTRAS) bst.o gst.o -o test-gst

test-avl :
	gcc $(LOPTS) -c avl.c bst.c $(EXTRAS) avl-0-0.c
	gcc $(LOPTS) avl-0-0.o $(OEXTRAS) bst.o avl.o -o test-avl

trees :
	gcc $(LOPTS) -c trees.c avl.c gst.c bst.c scanner.c $(EXTRAS)
	gcc $(LOPTS)  $(OEXTRAS) scanner.o bst.o gst.o avl.o trees.o -o trees


valgrind :
	echo testing singly-linked list
	valgrind ./test-sll
	echo
	echo testing doubly-linked list
	valgrind ./test-dll
	echo
	echo testing stack
	valgrind ./test-stack
	echo
	echo testing queue
	valgrind ./test-queue
	echo
	echo testing bst
	valgrind ./test-bst
	echo
	echo testing gst
	valgrind ./test-gst
	echo
	echo testing avl
	valgrind ./test-avl
	echo
	echo testing trees
	valgrind ./trees -v
	echo

test :
	./test-sll
	./test-dll
	./test-stack
	./test-queue
	./test-bst
	./test-gst
	./test-avl
	./trees -v

clean :
	rm -f $(OBJS) test-*.o bst-*-*.o test-bst test-sll test-dll test-stack test-queue test-gst test-avl trees

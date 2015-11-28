
CFLAGS=-c -Wall

fiboheap.o: fiboheap.c
	gcc $(CFLAGS) fiboheap.c

tests.o: tests.c
	gcc $(CFLAGS) tests.c -I$(CUNIT_DIR)/include

tests: tests.o fiboheap.o
	gcc -g tests.o fiboheap.o -L$(CUNIT_DIR)/lib -lcunit -o tests

test: tests
	./tests

check: tests
	valgrind --leak-check=yes ./tests

clean:
	rm *.o tests
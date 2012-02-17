CFLAGS=-std=gnu99 -Wall

all: jjy_test

jjy.o: jjy.c jjy.h
	gcc $(CFLAGS) -c jjy.c -o jjy.o
	
jjy_test: jjy_test.c jjy.o jjy.h
	gcc $(CFLAGS) jjy_test.c jjy.o -o jjy_test

.PHONY: clean test

clean:
	rm -f *.o jjy_test

test: clean jjy_test
	./jjy_test

all : test libgrepfree.so

test: test.c
	gcc test.c -o test

libgrepfree.so : grepfree.c
	gcc -shared -o libgrepfree.so -fPIC grepfree.c -ldl

all: clean test

clean:
	rm -f muses_72323_test

test:
	${CC} ${CFLAGS} -g muses_72323_test.c muses_72323.c -o muses_72323_test

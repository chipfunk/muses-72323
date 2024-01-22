all: test

test:
	echo ${CFLAGS}
	${CC} ${CFLAGS} -g muses_72323_test.c muses_72323.c -o muses_72323_test

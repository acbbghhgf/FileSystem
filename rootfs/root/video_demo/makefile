PWD=$(shell pwd)
src_dir=${PWD}/
src_c=$(shell find $(src_dir) -name "*.c")
objs=$(patsubst %.c,%.o, ${src_c})
OBJ=demo

CC=gcc
CFLAGS=-I . -O -g -pthread

all:${OBJ}
	echo ${OBJ}

${OBJ}:${objs}
	$(CC) $^ -o $@ ${CFLAGS}

%.o:%.c
	$(CC) ${CFLAGS} -c -o $@ $<;

clean:
	-rm ${objs} ${OBJ}



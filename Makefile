CC = gcc
CFLAGS = -Wall -Werror -march=core2 -O3 -g
INCLUDE= -I/home/sbowman/local/include 
LIBS= -L/home/sbowman/local/lib -lJudy
SOURCE = group.c ord_tup.c disk_queue.c disk_list.c bfs.c hashtable.c \
array_queue.c

% : %.c 
	${CC} ${CFLAGS} ${INCLUDE} ${LIBS} $^ -o $@

all : test main

clean:
	rm -f main test *.log *.o *~ nohup.out 

main: ${SOURCE} main.c

test: ${SOURCE} test.c

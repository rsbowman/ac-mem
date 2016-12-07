CC = clang++
CFLAGS = -Wall -Werror -march=native -O3 -g -std=c++11
SOURCE = group.cpp ord_tup.cpp disk_queue.cpp disk_list.cpp bfs.cpp hashtable.cpp array_queue.cpp

% : %.cpp
	${CC} ${CFLAGS} ${INCLUDE} ${LIBS} $^ -o $@

% : %.c
	${CC} ${CFLAGS} ${INCLUDE} ${LIBS} $^ -o $@

all : test main

clean:
	rm -f main test *.log *.o *~ nohup.out

main: ${SOURCE} main.cpp

test: ${SOURCE} test.cpp

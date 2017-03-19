PROGS = armemu
OBJS = sum_array_s.o
CFLAGS = -g

all : ${PROGS}

sum_array_s.o : sum_array_s.s
	as -o sum_array_s.o sum_array_s.s

armemu : armemu.c add.o sum_array_s.o
	gcc -o armemu armemu.c sum_array_s.o

clean:
	rm -rf ${PROGS} ${OBJS}


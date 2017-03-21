PROGS = armemu
OBJS = sum_array_s.o find_max_s.o fibo_iter_s.o fibo_rec_s.o find_sub_in_s_s.o
CFLAGS = -g

all : ${PROGS}

sum_array_s.o : sum_array_s.s
	as -o sum_array_s.o sum_array_s.s

find_max_s.o : find_max_s.s
	as -o find_max_s.o find_max_s.s

fibo_iter_s.o : fibo_iter_s.s
	as -o fibo_iter_s.o fibo_iter_s.s

fibo_rec_s.o : fibo_rec_s.s
	as -o fibo_rec_s.o fibo_rec_s.s

find_sub_in_s_s.o : find_sub_in_s_s.s
	as -o find_sub_in_s_s.o find_sub_in_s_s.s

armemu : armemu.c sum_array_s.o find_max_s.o fibo_iter_s.o fibo_rec_s.o find_sub_in_s_s.o
	gcc -o armemu armemu.c sum_array_s.o find_max_s.o fibo_iter_s.o fibo_rec_s.o find_sub_in_s_s.o

clean:
	rm -rf ${PROGS} ${OBJS}


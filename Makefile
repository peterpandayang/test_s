PROGS = testit
OBJS = sum_array_s.o find_max_s.o fibo_rec_s.o find_sub_in_s_s.o add
CFLAGS = -g

all : ${PROGS}

sum_array_s.o : sum_array_s.s
	as -o sum_array_s.o sum_array_s.s

find_max_s.o : find_max_s.s
	as -o find_max_s.o find_max_s.s

fibo_rec_s.o : fibo_rec_s.s
	as -o fibo_rec_s.o fibo_rec_s.s

find_sub_in_s_s.o : find_sub_in_s_s.s
	as -o find_sub_in_s_s.o find_sub_in_s_s.s

testit : testit.c sum_array_s.o find_max_s.o fibo_rec_s.o find_sub_in_s_s.o
	gcc -o testit testit.c sum_array_s.o find_max_s.o fibo_rec_s.o find_sub_in_s_s.o

clean :
	rm -rf ${PROGS} ${OBJS}

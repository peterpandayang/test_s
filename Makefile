PROGS = testit
OBJS = add.o ifelse.o sum_array_s.o find_max_s.o fibo_rec_s.o add
CFLAGS = -g

all : ${PROGS}

# first_a : first_a.s
# 	as -o first_a.o first_a.s
# 	gcc -o first_a first_a.o

ifelse.o : ifelse.s
	as -o ifelse.o ifelse.s

add.o : add.s
	as -o add.o add.s

sum_array_s.o : sum_array_s.s
	as -o sum_array_s.o sum_array_s.s

find_max_s.o : find_max_s.s
	as -o find_max_s.o find_max_s.s

fibo_rec_s.o : fibo_rec_s.s
	as -o fibo_rec_s.o fibo_rec_s.s

testit : testit.c add.o ifelse.o sum_array_s.o find_max_s.o fibo_rec_s.o
	gcc -o testit testit.c add.o ifelse.o sum_array_s.o find_max_s.o fibo_rec_s.o

clean :
	rm -rf ${PROGS} ${OBJS}

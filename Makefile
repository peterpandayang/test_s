PROGS = testit
OBJS = add.o ifelse.o add
CFLAGS = -g

all : ${PROGS}

# first_a : first_a.s
# 	as -o first_a.o first_a.s
# 	gcc -o first_a first_a.o

ifelse.o : ifelse.s
	as -o ifelse.o ifelse.s

add.o : add.s
	as -o add.o add.s

testit : testit.c add.o ifelse.s
	gcc -o testit testit.c add.o ifelse.o

clean :
	rm -rf ${PROGS} ${OBJS}

#include <stdio.h>

int add(int a, int b);

int ifelse(int a, int b);

int main(int argc, char **argv){
    int r;

    r = add(3,3);

    printf("r = %d\n", r);

    int rst = ifelse(1,3);
    printf("rst is: %d\n", rst);

    return 0;
}

	

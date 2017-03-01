#include <stdio.h>
#include <string.h>

#define VALUE_MAX_STR_LEN 64

struct value_st {
	int array[VALUE_MAX_STR_LEN];
};

int ifelse(int a, int b);

void init_array(int *p, int n){
	int i = 0;
	for(i = 0; i < n ; i++){
		p[i] = i;
		printf("this elem is: %d\n", p[i]);
	}
    p[i] = '\0';
}

// func for sum array
int sum_array_c(int *array, int n){
    int i;
    int sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + array[i]; 
    }
    return sum;
}

int finx_max_c(int *p, int n){
    int i = 0;
    int max = array[0];
    for (i = 1; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

int fibo_iter_c(int n){
    int prev = 0, curr = 1, next = 1;
    for (int i = 0; i < n; i++) {
        prev = curr;
        curr = next;
        next = prev + curr;
    }
    return prev;
}

int fibo_rec_c(int n){
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fib_rec(n - 1) + fib_rec(n - 2);
    }
}


int sum_array_s(int *p, int n);

int find_max_s(int *p, int n);

int fibo_rec_s(int n);


int main(int argc, char **argv){
    int r;

    struct value_st v_st;
    init_array(&(v_st.array), 10);

    int sum_s = sum_array_s(&(v_st.array), 10);
    printf("sum is: %d\n", sum_s);

    int max_s = find_max_s(&(v_st.array), 10);
    printf("max is: %d\n", max_s);

    int fibo_input = 5;

    int fibo_s = fibo_rec_s(fibo_input);
    printf("fibo is %d\n", fibo_s);

    return 0;
}

	








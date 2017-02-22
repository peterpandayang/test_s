#include <stdio.h>
#include <string.h>
// int add(int a, int b);

#define VALUE_MAX_STR_LEN 64

struct value_st {
	int array[VALUE_MAX_STR_LEN];
};

int ifelse(int a, int b);

// func for sum array

void init_array(int *p, int n){
	int i = 0;
	for(i = 0; i < n ; i++){
		p[i] = i;
		printf("this elem is: %d\n", p[i]);
	}
    p[i] = '\0';
}

int sum_array(int *array, int n){
    int i;
    int sum = 0;

    for (i = 0; i < n; i++) {
        sum = sum + array[i]; 
    }

    return sum;
}

int sum_array_s(int *p, int n);



int main(int argc, char **argv){
    int r;

    struct value_st v_st;
    init_array(&(v_st.array), 10);

    // int rst = ifelse(3,3);
    // printf("rst is: %d\n", rst);
    int sum = sum_array_s(&(v_st.array), 10);
    printf("v_st.array is: %d\n", v_st.array);
    printf("sum is: %d\n", sum);

    return 0;
}

	








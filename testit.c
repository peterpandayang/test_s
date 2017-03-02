#include <stdio.h>
#include <string.h>

#define VALUE_MAX_STR_LEN 64

struct value_st {
    int array[VALUE_MAX_STR_LEN];
    char s[50];
    char sub[10];
};


void init_array_c(int *p, int n){
    int i = 0;
    for(i = 0; i < n ; i++){
	p[i] = i;
        printf("this elem is: %d\n", p[i]);
    }
    p[i] = '\0';
}

int sum_array_c(int *array, int n){
    int i;
    int sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + array[i]; 
    }
    return sum;
}

int find_max_c(int *array, int n){
    int i;
    int max = array[0];
    for (i = 1; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max; 
}

int fibo_iter_c(int n) {
    int prev = 0, curr = 1, next = 1;
    int i;
    for (i = 0; i < n; i++) {
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
        return fibo_rec_c(n - 1) + fibo_rec_c(n - 2);
    }
}

int find_sub_in_s_c(char *s, char *sub){
    char *p, *q;
    int i, len = strlen(s) - strlen(sub);
    for (i = 0; i <= len; i++) {
        p = s + i;
        q = sub;
        if (*q && *p == *q) {
            if(check_commom(p, q) == 0){
                return i;
            }
        }
    }
    return -1;
}

int check_commom(char *s, char *sub){
    int len = strlen(sub);
    int i;
    char *p, *q;
    p = s;
    q = sub;
    for(i = 0; i < len; i++){
        if(*q != *p){
            return -1;
        }
    }
    return 0;
}

int sum_array_s(int *p, int n);

int find_max_s(int *p, int n);

int fibo_rec_s(int n);

int find_sub_in_s_s(char *s, char *sub);

int main(int argc, char **argv){
    struct value_st v_st;
    int *p_array = v_st.array;
    strcpy(v_st.s, "This is a test string for testing");
    strcpy(v_st.sub, "test");
    char *p_s = v_st.s;
    char *p_sub = v_st.sub;
    
    init_array_c(p_array, 10);

    int sum_s = sum_array_s(p_array, 10);
    printf("sum of assemly is: %d\n", sum_s);

    int sum_c = sum_array_c(p_array, 10);
    printf("sum of c is: %d\n", sum_c);

    int max_s = find_max_s(p_array, 10);
    printf("max of assembly is: %d\n", max_s);

    int max_c = find_max_c(p_array, 10);
    printf("max of c is: %d\n", max_c);

    int fibo_input = 5;

    int iter_c = fibo_iter_c(fibo_input);
    printf("iteration fibo with input %d for c is: %d\n", fibo_input, iter_c);

    int rec_s = fibo_rec_s(fibo_input);
    printf("recursion fibo with input %d for assembly is: %d\n", fibo_input, rec_s);

    int rec_c = fibo_rec_c(fibo_input);
    printf("recursion fibo with input %d for c is: %d\n", fibo_input, rec_c);

    int pos;
    pos = find_sub_in_s_c(p_s, p_sub);
    if(pos != -1){
        printf("find sub in s starting from position: %d for c\n", pos);
    }
    else{
        printf("can't find sub in s for c");
    }

    pos = 0;
    pos = find_sub_in_s_s(p_s, p_sub);
    if(pos != -1){
        printf("find sub in s starting from position: %d for assembly\n", pos);
    }
    else{
        printf("can't find sub in s for assembly");
    }

    return 0;
}

	








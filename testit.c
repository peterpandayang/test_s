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
    printf("Array with positive input: \n");
    for(i = 0; i < n ; i++){
	    p[i] = i;
        printf("%d ", p[i]);
    }
    printf("\n");
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
        if (*q == *p) {
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
        p = p + 1;
        q = q + 1;
    }
    return 0;
}

int sum_array_s(int *p, int n);

int find_max_s(int *p, int n);

int fibo_iter_s(int n);

int fibo_rec_s(int n);

int find_sub_in_s_s(char *s, char *sub);

int positive_test(int *p_array, int size){
    init_array_c(p_array, size);

    printf("Test for array with positive values: \n");

    int sum_s = sum_array_s(p_array, size);
    printf("Sum result are: %d (Assembly) ", sum_s);

    int sum_c = sum_array_c(p_array, size);
    printf(", %d (C)\n", sum_c);

    int max_s = find_max_s(p_array, size);
    printf("Max result are: %d (Assembly)", max_s);

    int max_c = find_max_c(p_array, size);
    printf(", %d (C)\n", max_c);

    return 0;
}

int fibo_test(int size){
    int i;
    int iter_s;
    int iter_c;
    int rec_s;
    int rec_c;

    printf("Test for fibonacci sequence of iteration with size %d: \n", size + 1);
    for(i = 0; i < size; i++){
        int iter_s = fibo_iter_s(i);
        printf("%d ", iter_s);
    }
    printf("(Assembly)\n");

    for(i = 0; i < size; i++){
        int iter_c = fibo_iter_c(i);
        printf("%d ", iter_c);
    }
    printf("(C)\n");

    printf("Test for fibonacci sequence of recursion with size %d: \n", size + 1);
    for(i = 0; i < size; i++){
        int rec_s = fibo_rec_s(i);
        printf("%d ", rec_s);
    }
    printf("(Assembly)\n");

    for(i = 0; i < size; i++){
        int rec_c = fibo_rec_c(i);
        printf("%d ", rec_c);
    }
    printf("(C)\n");
    return 0;
}

int strstr_test(char *p_s, char *p_sub){
    int pos; 
    printf("Larger string: \n");
    print_str(p_s);
    printf("Smaller string: \n");
    print_str(p_sub);
    printf("Test for strstr in C: \n");

    pos = find_sub_in_s_c(p_s, p_sub);
    if(pos != -1){
        printf("Find substring at:\n");
    }
    else{
        printf("Can't find substring\n");
    }

    printf("Test for strstr in Assembly: \n");
    pos = 0;
    pos = find_sub_in_s_s(p_s, p_sub);
    if(pos != -1){
        printf("Find substring at:\n");
    }
    else{
        printf("Can't find substring\n");
    }
    return 0;
}

int print_str(char *p){
    int len = strlen(p);
    int i;
    for(i = 0; i < len; i++){
        printf("%s\n", *p);
        p += 1;
    }
}

int main(int argc, char **argv){
    struct value_st v_st;
    int *p_array = v_st.array;
    strcpy(v_st.s, "This is a test string for testing");
    strcpy(v_st.sub, "test");
    char *p_s = v_st.s;
    char *p_sub = v_st.sub;
    
    positive_test(p_array, 20);

    fibo_test(19);

    strstr_test(p_s, p_sub);

    return 0;
}

	








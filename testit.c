#include <stdio.h>
#include <string.h>
#define VALUE_MAX_STR_LEN 64

struct value_st {
    char s[55];
    char sub[10];
	int array[VALUE_MAX_STR_LEN];
};


void init_array(int *p, int n){
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
    for (int i = 0; i < n; i++) {
        prev = curr;
        curr = next;
        next = prev + curr;
    }
    return prev;
}

int fib_rec_c(int n){
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fib_rec_c(n - 1) + fib_rec_c(n - 2);
    }
}

// int find_str(char *s, char *sub){
//     char *p, *q;
//     int i, len = strlen(s) - strlen(sub);

//     for (i = 0; i <= len; i++) {
//         p = s + i;
//         q = sub;
//         while (*q && *p == *q) {
//             p += 1;
//             q += 1;
//         }
//         if (*q == 0)
//             return i;
//         printf("%s\n", q);
//     }
//     return -1;
// }

int find_sub_in_s(char *s, char *sub){
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
        if(*q && *p != *q){
            return -1;
        }
    }
    return 0;
}



int sum_array_s(int *p, int n);

int find_max_s(int *p, int n);

int fibo_rec_s(int n);


int main(int argc, char **argv){
    int r;

    struct value_st v_st;
    strcpy(v_st.s, "This is a test string for testing");
    strcpy(v_st.sub, "test");
    char *p_s = v_st.s;
    char *p_sub = v_st.sub;
    init_array(&(v_st.array), 10);

    // int sum = sum_array_s(&(v_st.array), 10);
    // printf("sum is: %d\n", sum);

    // int max = find_max_s(&(v_st.array), 10);
    // printf("max is: %d\n", max);

    // int fibo = fibo_rec_s(5);
    // printf("fibo is %d\n", fibo);

    

    // int *p1 = &s;
    // int *p2 = &sub;
    int p;
    p = find_sub_in_s(p_s, p_sub);
    printf("%d\n", p);
    // if(p)
    // {
    //     printf("string found\n" );
    //     printf ("First occurrence of string \"test\" in \"%s\" is"\" \"%s\"",string, p);
    // }
    // else {printf("string not found\n" );}

    return 0;
}

	








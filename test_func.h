
void single_sum_array_test(struct arm_state *state, unsigned int *func, int *p_array, int size, int index){
    if(size != 1000){
        print_array_c(p_array, size);
    }
    else{
        printf("This is input of 1000 element from 0 to 999\n");
    }
    init_arm_state(state, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
    int sum;
    sum = sum_array_s(p_array, size);
    printf("Sum is: %d (Assembly)\n", sum);
    sum = armemu(state);
    printf("Sum is: %d (Armemu)\n\n", sum);
    gettime_array(state, (unsigned int *) func, p_array, size, 1);
    print_analysis(state);
    write_to_output(state, 1 + index);
}

void sum_array_test(struct arm_state *state, unsigned int *func, int *p_array, int *p_neg_array, int *p_zero_array, int *p_large_array, int size){
    printf("Start sum array test and print input array:\n");
    printf("Test for array with positive values: \n");
    single_sum_array_test(state, (unsigned int *) func, p_array, size, 0);
    printf("Test for array with negative values: \n");
    single_sum_array_test(state, (unsigned int *) func, p_neg_array, size, 1);
    printf("Test for array with zero values: \n");
    single_sum_array_test(state, (unsigned int *) func, p_zero_array, size, 2);
    printf("Test for array with large size: \n");
    single_sum_array_test(state, (unsigned int *) func, p_large_array, 1000, 3);
}      

void single_find_max_test(struct arm_state *state, unsigned int *func, int *p_array, int size, int index){
    if(size != 1000){
        print_array_c(p_array, size);
    }
    else{
        printf("This is input of 1000 element from 0 to 999\n");
    }
    init_arm_state(state, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
    int max;
    max = find_max_s(p_array, size);
    printf("Max is: %d (Assembly)\n", max);
    max = armemu(state);
    printf("Max is: %d (Armemu)\n\n", max);
    gettime_array(state, (unsigned int *) func, p_array, size, 1);
    print_analysis(state);
    write_to_output(state, 5 + index);
}            

void find_max_test(struct arm_state *state, unsigned int *func, int *p_array, int *p_neg_array, int *p_zero_array, int *p_large_array, int size){
    printf("Start max array test and print input array:\n");
    printf("Test for array with positive values: \n");
    single_find_max_test(state, (unsigned int *) func, p_array, size, 0);
    printf("Test for array with negative values: \n");
    single_find_max_test(state, (unsigned int *) func, p_neg_array, size, 1);
    printf("Test for array with zero values: \n");
    single_find_max_test(state, (unsigned int *) func, p_zero_array, size, 2);
    printf("Test for array with large size: \n");
    single_find_max_test(state, (unsigned int *) func, p_large_array, 1000, 3);
}

void signle_fibo_iter_test(struct arm_state *state, unsigned int *func, int number, int index){
    init_arm_state(state, (unsigned int *) func, number, 0, 0, 0);
    int fibo_iter;
    fibo_iter = armemu(state);
    printf("Fibo iteration result for %d's element is:\n", number);
    gettime_fibo(state, (unsigned int *) func, number, 1);
    print_analysis(state);
    write_to_output(state, index);
}

void fibo_iter_test(struct arm_state *state, unsigned int *func, int size){
    printf("Start iteration fibonacci test:\n");
    printf("Test for fibonacci sequence of iteration with size %d: \n", size);
    int i;
    for(i = 0; i < 20; i++){
        int iter_s = fibo_iter_s(i);
        printf("%d ", iter_s);
    }
    printf("(Assembly)\n");
    for(i = 0; i < 20; i++){
        init_arm_state(state, (unsigned int *) func, i, 0, 0, 0);
        int iter_arm = armemu(state);
        printf("%d ", iter_arm);
    }
    printf("(Armemu)\n");
    printf("\n");
    signle_fibo_iter_test(state, (unsigned int *) func, 10, 10);
    signle_fibo_iter_test(state, (unsigned int *) func, 19, 19);
}

void signle_fibo_rec_test(struct arm_state *state, unsigned int *func, int number, int index){
    init_arm_state(state, (unsigned int *) func, number, 0, 0, 0);
    int fibo_rec;
    fibo_rec = armemu(state);
    printf("Fibo recursion result for %d's element is:\n", number);
    gettime_fibo(state, (unsigned int *) func, number, 2);
    print_analysis(state);
    write_to_output(state, index);
}

void fibo_rec_test(struct arm_state *state, unsigned int *func, int size){
    printf("Start recursion fibonacci test:\n");
    printf("Test for fibonacci sequence of recursion with size %d: \n", size);
    int i;
    for(i = 0; i < 20; i++){
        int rec_s = fibo_rec_s(i);
        printf("%d ", rec_s);
    }
    printf("(Assembly)\n");

    for(i = 0; i < 20; i++){
        init_arm_state(state, (unsigned int *) func, i, 0, 0, 0);
        int rec_arm = armemu(state);
        printf("%d ", rec_arm);
    }
    printf("(Armemu)\n");
    printf("\n");
    signle_fibo_rec_test(state, (unsigned int *) func, 10, 11);
    signle_fibo_rec_test(state, (unsigned int *) func, 19, 20);
}

void find_sub_in_s_test(struct arm_state *state, unsigned int *func, char *p_s, char *p_sub){
    printf("Start strstr test:\n");
    printf("Larger string: \n");
    print_str(p_s);
    printf("Smaller string: \n");
    print_str(p_sub);
    unsigned int int_p_s = (unsigned int)((unsigned int *)p_s);
    unsigned int int_p_sub = (unsigned int)((unsigned int *)p_sub);
    int s_len = strlen(p_s);
    int s_sub_len = strlen(p_sub);
    int pos;
    pos = find_sub_in_s_s(p_s, p_sub, s_len, s_sub_len);
    printf("Start position is: %d (Assembly)\n", pos);
    init_arm_state(state, (unsigned int *) func, int_p_s, int_p_sub, s_len, s_sub_len);
    pos = armemu(state);
    printf("Start position is: %d (Armemu)\n\n", pos);
    gettime_find_s_in_sub(state, (unsigned int *) func, int_p_s, int_p_sub, s_len, s_sub_len);
    print_analysis(state);
    write_to_output(state, 9);
}
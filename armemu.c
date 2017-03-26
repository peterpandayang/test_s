#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#define NREGS 16
#define STACK_SIZE 1024
#define SP 13
#define LR 14
#define PC 15
#define VALUE_MAX_STR_LEN 64 
#define ITERS_ARRAY 1000
#define ITERS_FIBO 10
#define ITERS_FIND_SUB_IN_S 1000  
#define ITERS_ASSEM 10000

int sum_array_s(int *p, int n);
int find_max_s(int *p, int n);
int fibo_iter_s(int n);
int fibo_rec_s(int n);
int find_sub_in_s_s(char *s, char *sub, int s_len, int s_sub_len);

struct arm_state {
    unsigned int regs[NREGS];
    unsigned int cpsr;
    unsigned char stack[STACK_SIZE];
    int array[VALUE_MAX_STR_LEN];
    char s[50];
    char sub[10];
    int exec_instr_count;
    int compu_count;
    int mem_count;
    int branch_count;
    int b_taken_count;
    int b_not_taken_count;
    int reg_reading_count;
    int reg_writing_count;
    unsigned int read_regs[NREGS + 1];
    unsigned int written_regs[NREGS + 1];
    double armemu_total_time_usecs;
    double armemu_total_time_secs;
    double native_total_time_usecs;
    double native_total_time_secs;
};

void init_arm_state(struct arm_state *state, unsigned int *func, unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3){
    int i;
    /* zero out all arm state */
    for (i = 0; i < NREGS; i++) {
        state->regs[i] = 0;
        state->read_regs[i] = 0;
        state->written_regs[i] = 0;
    }
    state->read_regs[NREGS] = 0;
    state->written_regs[NREGS] = 0;
    state->cpsr = 0;
    for (i = 0; i < STACK_SIZE; i++) {
        state->stack[i] = 0;
    }
    state->regs[PC] = (unsigned int) func;
    state->regs[SP] = (unsigned int) &state->stack[STACK_SIZE];
    state->regs[LR] = 0;
    state->regs[0] = arg0;
    state->regs[1] = arg1;
    state->regs[2] = arg2;
    state->regs[3] = arg3;
    state->exec_instr_count = 0;
    state->compu_count = 0;
    state->mem_count = 0;
    state->branch_count = 0;
    state->b_taken_count = 0;
    state->b_not_taken_count = 0;
}

void init_array_c(int *p_pos, int n){
    int i = 0;
    for(i = 0; i < n ; i++){
        p_pos[i] = i;
    }
    p_pos[i] = '\0';
}

void update_read_regs(struct arm_state *state, int i){
    state->read_regs[i] = 1;
}

void update_written_regs(struct arm_state *state, int i){
    state->written_regs[i] = 1;
}

/* data processing part*/
bool is_add_inst(unsigned int iw){
    unsigned int opcode;
    opcode = (iw >> 21) & 0b1111;
    return (opcode == 0b0100);
}

bool is_sub_inst(unsigned int iw){
    unsigned int opcode;
    opcode = (iw >> 21) & 0b1111;
    return (opcode == 0b0010);
}

bool is_imme_dp(unsigned int iw){
    unsigned int i;
    i = (iw >> 25) & 0b1;
    return i == 0b1;
}

void armemu_add(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm, add_value;
    iw = *((unsigned int *) state->regs[PC]); 
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;
    if(is_imme_dp(iw)){
        add_value = iw & 0xFF;
    }
    else{
        rm = iw & 0xF;
        add_value = state->regs[rm];
        update_read_regs(state, rm);
    }
    state->regs[rd] = state->regs[rn] + add_value; 
    update_read_regs(state, rn);
    update_written_regs(state, rd); 
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
        update_written_regs(state, PC);
    }
}

void armemu_sub(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm, sub_value;
    iw = *((unsigned int *) state->regs[PC]);    
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;
    if(is_imme_dp(iw)){
        sub_value = iw & 0xFF;
    }
    else{
        rm = iw & 0xF;
        sub_value = state->regs[rm];
        update_read_regs(state, rm);
    }  
    state->regs[rd] = state->regs[rn] - sub_value;
    update_written_regs(state, rd);
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
        update_written_regs(state, PC);
    }
}

bool is_cmp_inst(unsigned int iw){
    unsigned int opcode;
    opcode = (iw >> 21) & 0b1111;
    return (opcode == 0b1010);
}

void update_cpsr_cmp(struct arm_state *state, int val1, int val2){
    if(val1 - val2 < 0){
        update_written_regs(state, 16);
        state->cpsr = 0x80000000;
    }
    if(val1 - val2 == 0){
        update_written_regs(state, 16);
        state->cpsr = 0x40000000;
    }
}

void armemu_cmp(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm, cmp_value;
    iw = *((unsigned int *) state->regs[PC]);  
    rn = (iw >> 16) & 0xF;
    state->cpsr = 0;
    if(is_imme_dp(iw)){
        cmp_value = iw & 0xFF;
    }
    else{
        rm = iw & 0xF;
        cmp_value = state->regs[rm];
        update_read_regs(state, rm);
    }
    update_cpsr_cmp(state, state->regs[rn], cmp_value);
    update_written_regs(state, PC);
    state->regs[PC] = state->regs[PC] + 4;
}

bool is_mov_inst(unsigned int iw){
    unsigned int opcode;
    opcode = (iw >> 21) & 0b1111;
    return (opcode == 0b1101);
}

void armemu_mov(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, imme;
    iw = *((unsigned int *) state->regs[PC]);
    rd = (iw >> 12) & 0xF;
    if(is_imme_dp(iw)){
        unsigned int imme = iw & 0xFF;
        state->regs[rd] = imme;
    }
    else{
        rn = iw & 0b1111;
        state->regs[rd] = state->regs[rn];
        update_read_regs(state, rn);
    }
    update_written_regs(state, rd);
    if (rd != PC) {
        update_written_regs(state, PC);
        state->regs[PC] = state->regs[PC] + 4;
    }
}

bool is_data_pro_inst(unsigned int iw){
    unsigned int op;
    op = (iw >> 26) & 0b11;
    return op == 0;
}

void armemu_data_pro(struct arm_state *state){
    unsigned int iw;
    iw = *((unsigned int *) state->regs[PC]);
    if(is_mov_inst(iw)){
        armemu_mov(state);
    }
    else if(is_add_inst(iw)){
        armemu_add(state);
    }  
    else if(is_sub_inst(iw)){
        armemu_sub(state);
    }  
    else if(is_cmp_inst(iw)){
        armemu_cmp(state);
    }
}

/*memory part*/
bool is_ldr_inst(unsigned int iw){
    unsigned int L, B;
    L = (iw >> 20) & 0b1;
    B = (iw >> 22) & 0b1;
    return L == 0b1 && B == 0b0;
}

bool is_ldrb_inst(unsigned int iw){
    unsigned int L, B;
    L = (iw >> 20) & 0b1;
    B = (iw >> 22) & 0b1;
    return L == 0b1 && B == 0b1;
}

bool is_str_inst(unsigned int iw){
    unsigned int L, B;
    L = (iw >> 20) & 0b1;
    B = (iw >> 22) & 0b1;
    return L == 0b0 && B == 0b0;
}

bool is_off_addr(unsigned int iw){
    unsigned int P, W;
    P = (iw >> 24) & 0b1;
    W = (iw >> 21) & 0b1;
    return P == 0b1 && W == 0b0;
}

void armemu_ldr(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, offset, i;
    iw = *((unsigned int *) state->regs[PC]);
    rn = (iw >> 16) & 0xF;
    rd = (iw >> 12) & 0xF;
    if(is_off_addr(iw)){
        i = iw >> 25 & 0b1;
        if(i == 0b0){
            offset = iw & 0xFFF;
            state->regs[rd] = *((unsigned int *)(state->regs[rn] + offset));
            update_written_regs(state, rd);  
            update_read_regs(state, rn);  
        }        
    }
    if (rd != PC) {
        update_written_regs(state, PC);
        state->regs[PC] = state->regs[PC] + 4;
    }
}

void armemu_ldrb(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, offset, i;
    iw = *((unsigned int *) state->regs[PC]);
    rn = (iw >> 16) & 0xF;
    rd = (iw >> 12) & 0xF;
    if(is_off_addr(iw)){
        i = iw >> 25 & 0b1;
        if(i == 0b0){
            offset = iw & 0xFFF;
            state->regs[rd] = *((unsigned int *)(state->regs[rn] + offset)) & 0xFF;
            update_written_regs(state, rd);  
            update_read_regs(state, rn);  
        }        
    }
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
        update_written_regs(state, PC);
    }
}

void armemu_str(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, offset, i;
    iw = *((unsigned int *) state->regs[PC]);
    rn = (iw >> 16) & 0xF;
    rd = (iw >> 12) & 0xF;
    if(is_off_addr(iw)){
        i = iw >> 25 & 0b1;
        if(i == 0b0){
            offset = iw & 0xFFF;
            *((unsigned int *)(state->regs[rn] + offset)) = state->regs[rd];
            update_written_regs(state, rn);  
            update_read_regs(state, rd);  
        }        
    }
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
        update_written_regs(state, PC);
    }
}

bool is_mem_inst(unsigned int iw){
    unsigned int op;
    op = (iw >> 26) & 0b11;
    return op == 0b01;
}

void armemu_mem(struct arm_state *state){
    unsigned int iw;
    iw = *((unsigned int *)state->regs[PC]);
    if(is_ldr_inst(iw)){
        armemu_ldr(state);
    }
    else if(is_str_inst(iw)){
        armemu_str(state);
    }
    else if(is_ldrb_inst(iw)){
        armemu_ldrb(state);
    }
}

/* branch part*/
bool is_bx_inst(unsigned int iw){
    unsigned int bx_code;
    bx_code = (iw >> 4) & 0x00FFFFFF;
    return (bx_code == 0b000100101111111111110001);
}

void armemu_bx(struct arm_state *state){
    unsigned int iw;
    unsigned int rn;
    iw = *((unsigned int *) state->regs[PC]);
    update_read_regs(state, PC); 
    rn = iw & 0b1111;
    state->regs[PC] = state->regs[rn];
    update_written_regs(state, PC); 
}

bool is_b_inst(unsigned int iw){
    unsigned int op;
    op = (iw >> 26) & 0b10;
    return op == 0b10;
}

bool is_beq_inst(unsigned int iw){
    unsigned int cond;
    cond = iw >> 28 & 0b1111;
    return cond == 0b0000;
}

bool is_bne_inst(unsigned int iw){
    unsigned int cond;
    cond = iw >> 28 & 0b1111;
    return cond == 0b0001;
}

bool is_b_default_inst(unsigned int iw){
    unsigned int cond;
    cond = iw >> 28 & 0b1111;
    return cond == 0b1110;
}

bool is_neg_offset(unsigned int iw){
    unsigned int sign;
    sign = iw >> 23 & 0b1;
    return sign == 0b1;
}

bool is_bge_inst(unsigned int iw){
    unsigned int bge;
    bge = iw >> 28 & 0b1111;
    return bge == 0b1010;
}

bool is_bl_inst(unsigned int iw){
    unsigned int bl;
    bl = iw >> 24 & 0b1;
    return bl == 0b1;
}

void save_link_addr(struct arm_state *state){
    state->regs[LR] = state->regs[PC] + 4;
    update_written_regs(state, LR); 
}

void update_pc_ne_ge(int check_val, struct arm_state *state, int offset){
    if(check_val == 0b0){
        state->b_taken_count += 1;
        state->regs[PC] = state->regs[PC] + 8 + offset * 4;
    }
    else{
        state->b_not_taken_count += 1;
        state->regs[PC] = state->regs[PC] + 4;
    }
}

void update_pc_eq(int check_val, struct arm_state *state, int offset){
    if(check_val == 0x40000000){
        state->b_taken_count += 1;
        state->regs[PC] = state->regs[PC] + 8 + offset * 4;
    }
    else{
        state->b_not_taken_count += 1;
        state->regs[PC] = state->regs[PC] + 4;
    }    
}

void update_pc_bl(int iw, struct arm_state *state, int offset){
    if(is_beq_inst(iw)){
        if(state->cpsr == 0x40000000){
            save_link_addr(state);
        }
        update_pc_eq(state->cpsr, state, offset);
    }
    else{ 
        state->b_taken_count += 1;           
        save_link_addr(state);
        state->regs[PC] = state->regs[PC] + 8 + offset * 4;
    }
}

void armemu_b(struct arm_state *state){
    unsigned int iw, offset;
    iw = *((unsigned int *) state->regs[PC]);
    if(is_neg_offset(iw)){
        offset = 0xFFFFFF - (iw & 0xFFFFFF) + 1;
        offset = -offset;
    }
    else{
        offset = iw & 0xFFFFFF;
    }
    if(is_bl_inst(iw)){
        update_pc_bl(iw, state, offset);
    }
    else if(is_beq_inst(iw)){
        update_pc_eq(state->cpsr, state, offset);
    }
    else if(is_bge_inst(iw)){
        update_pc_ne_ge(state->cpsr >> 31, state, offset);
    }
    else if(is_bne_inst(iw)){
        update_pc_ne_ge(state->cpsr >> 30, state, offset);
    }
    else if(is_b_default_inst(iw)){
        state->b_taken_count += 1;
        state->regs[PC] = state->regs[PC] + 8 + offset * 4;
    }
    state->cpsr = 0;
    update_written_regs(state, PC); 
}

/*armemu part*/
void armemu_one(struct arm_state *state){
    unsigned int iw;   
    iw = *((unsigned int *) state->regs[PC]);
    update_read_regs(state, PC); 
    if (is_bx_inst(iw)) {
        state->branch_count += 1;
        state->b_taken_count += 1;
        armemu_bx(state);
    } 
    else if (is_b_inst(iw)) {
        state->branch_count += 1;
        armemu_b(state);
    } 
    else if(is_data_pro_inst(iw)){
        state->compu_count += 1;
        armemu_data_pro(state);
    }
    else if(is_mem_inst(iw)){
        state->mem_count += 1;
        armemu_mem(state);
    }
}

unsigned int armemu(struct arm_state *state){
    while (state->regs[PC] != 0) {
        armemu_one(state);
        state->exec_instr_count += 1;
    }
    return state->regs[0];
}

/*print part*/
void print_array_c(int *p_array, int n){
    int i = 0;
    for(i = 0; i < n ; i++){
        printf("%d ", p_array[i]);
    }
    printf("\n");
}

int print_str(char *p){
    int len = strlen(p);
    int i;
    printf("\"");
    for(i = 0; i < len; i++){
        printf("%c", *p);
        p += 1;
    }
    printf("\"");
    printf("\n");
    return 0;
}

void print_one_reg(struct arm_state *state, int i){
    if(i == 13){
        printf("sp");
    }
    else if(i == 14){
        printf("lr");
    }
    else if(i == 15){
        printf("pc");
    }
    else if(i == 16){
        printf("cpsr");
    }
    else{
        printf("r%d", i);
    }
}

void write_one_reg(struct arm_state *state, int i, FILE *f){
    if(i == 13){
        fprintf(f, "sp");
    }
    else if(i == 14){
        fprintf(f, "lr");
    }
    else if(i == 15){
        fprintf(f, "pc");
    }
    else if(i == 16){
        fprintf(f, "cpsr");
    }
    else{
        fprintf(f, "r%d", i);
    }
}

void print_read_regs(struct arm_state *state){
    int i;
    for(i = 0; i <= NREGS; i++){
        if(state->read_regs[i] == 1){
            print_one_reg(state, i);
            printf(" ");
        }
    }
    printf("\n");
}

void print_written_regs(struct arm_state *state){
    int i;
    for(i = 0; i <= NREGS; i++){
        if(state->written_regs[i] == 1){
            print_one_reg(state, i);
            printf(" ");
        }
    }
    printf("\n");
}

void write_read_regs_to_file(struct arm_state *state, FILE *f){
    int i;
    fprintf(f, "Register used as read: \n");
    for(i = 0; i <= NREGS; i++){
        if(state->read_regs[i] == 1){
            write_one_reg(state, i, f);
            fprintf(f, " ");
        }
    }
    fprintf(f, "\n");
}

void write_written_regs_to_file(struct arm_state *state, FILE *f){
    int i;
    fprintf(f, "Register used as written: \n");
    for(i = 0; i <= NREGS; i++){
        if(state->written_regs[i] == 1){
            write_one_reg(state, i, f);
            fprintf(f, " ");
        }
    }
    fprintf(f, "\n");
}

void print_analysis(struct arm_state *state){
    printf("Total instructions: %d\n", state->exec_instr_count);
    printf("Total computation instructions: %d\n", state->compu_count);
    printf("Total memory instructions: %d\n", state->mem_count);
    printf("Total branch instructions: %d\n", state->branch_count);
    printf("Total branch taken: %d\n", state->b_taken_count);
    printf("Total branch not taken: %d\n", state->b_not_taken_count);
    printf("Register used as read: \n");
    print_read_regs(state);
    printf("Register used as written: \n");
    print_written_regs(state);
    printf("armemu_total_time_usecs = %lf us\n", state->armemu_total_time_usecs);  
    printf("native_total_time_usecs = %lf us\n\n", state->native_total_time_usecs);  
}

void gettime_array(struct arm_state *state, unsigned int *func, int *p_array, int size, int index){
    struct timespec t1, t2;
    int i;
    long total_nsecs = 0;
    time_t total_secs = 0;
    double total_time_secs = 0.0;
    double total_time_usecs = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (i = 0; i < ITERS_ARRAY; i++) {
        init_arm_state(state, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
        armemu(state);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    total_secs = t2.tv_sec - t1.tv_sec;
    total_nsecs = t2.tv_nsec - t1.tv_nsec;
    total_time_secs = (double) total_secs + ((double) total_nsecs) / 1000000000.0;
    total_time_usecs = (((double) total_time_secs) / ((double) ITERS_ARRAY)) * 1000000.0;
    state->armemu_total_time_usecs = total_time_usecs;
    state->armemu_total_time_secs = total_time_secs;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    if(index == 1){
        for (i = 0; i < ITERS_ASSEM; i++) {
            sum_array_s(p_array, size);
        }       
    }
    else{
        for (i = 0; i < ITERS_ASSEM; i++) {
            find_max_s(p_array, size);
        } 
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    total_secs = t2.tv_sec - t1.tv_sec;
    total_nsecs = t2.tv_nsec - t1.tv_nsec;
    total_time_secs = (double) total_secs + ((double) total_nsecs) / 1000000000.0;
    total_time_usecs = (((double) total_time_secs) / ((double) ITERS_ASSEM)) * 10000000.0;
    state->native_total_time_secs = total_time_secs;
    state->native_total_time_usecs = total_time_usecs;
}

void gettime_fibo(struct arm_state *state, unsigned int *func, int size, int index){
    struct timespec t1, t2;
    int i;
    long total_nsecs = 0;
    time_t total_secs = 0;
    double total_time_secs = 0.0;
    double total_time_usecs = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (i = 0; i < ITERS_FIBO; i++) {
        init_arm_state(state, (unsigned int *) func, size, 0, 0, 0);
        armemu(state);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    total_secs = t2.tv_sec - t1.tv_sec;
    total_nsecs = t2.tv_nsec - t1.tv_nsec;
    total_time_secs = (double) total_secs + ((double) total_nsecs) / 1000000000.0;
    total_time_usecs = (((double) total_time_secs) / ((double) ITERS_FIBO)) * 1000000.0;
    state->armemu_total_time_usecs = total_time_usecs;
    state->armemu_total_time_secs = total_time_secs;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    if(index == 1){
        for (i = 0; i < ITERS_ASSEM; i++) {
            fibo_iter_s(size);
        }       
    }
    else{
        for (i = 0; i < ITERS_ASSEM; i++) {
            fibo_rec_s(size);
        } 
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    total_secs = t2.tv_sec - t1.tv_sec;
    total_nsecs = t2.tv_nsec - t1.tv_nsec;
    total_time_secs = (double) total_secs + ((double) total_nsecs) / 1000000000.0;
    total_time_usecs = (((double) total_time_secs) / ((double) ITERS_ASSEM)) * 10000000.0;
    state->native_total_time_secs = total_time_secs;
    state->native_total_time_usecs = total_time_usecs;
}

void gettime_find_s_in_sub(struct arm_state *state, unsigned int *func, int p_s, int p_sub, int s_len, int s_sub_len){
    struct timespec t1, t2;
    int i;
    long total_nsecs = 0;
    time_t total_secs = 0;
    double total_time_secs = 0.0;
    double total_time_usecs = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (i = 0; i < ITERS_FIND_SUB_IN_S; i++) {
        init_arm_state(state, (unsigned int *) func, p_s, p_sub, s_len, s_sub_len);
        armemu(state);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    total_secs = t2.tv_sec - t1.tv_sec;
    total_nsecs = t2.tv_nsec - t1.tv_nsec; 
    total_time_secs = (double) total_secs + ((double) total_nsecs) / 1000000000.0;
    total_time_usecs = (((double) total_time_secs) / ((double) ITERS_FIND_SUB_IN_S)) * 1000000.0;
    state->armemu_total_time_usecs = total_time_usecs;
    state->armemu_total_time_secs = total_time_secs;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (i = 0; i < ITERS_ASSEM; i++) {
        find_sub_in_s_s((char *)p_s, (char *)p_sub, s_len, s_sub_len);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    total_secs = t2.tv_sec - t1.tv_sec;
    total_nsecs = t2.tv_nsec - t1.tv_nsec;
    total_time_secs = (double) total_secs + ((double) total_nsecs) / 1000000000.0;
    total_time_usecs = (((double) total_time_secs) / ((double) ITERS_ASSEM)) * 10000000.0;
    state->native_total_time_secs = total_time_secs;
    state->native_total_time_usecs = total_time_usecs;
}

void write_inst_percentage(FILE *f, struct arm_state *state){
    fprintf(f, "\nTable for percentage of instructions\n");
    fprintf(f, "\nComputation      Memory       Branches\n");
    int total_instr = state->exec_instr_count;
    float computation_perc = (float)state->compu_count / (float)total_instr;
    float memory_perc = (float)state->mem_count / (float)total_instr;
    float branch_perc = (float)state->branch_count / (float)total_instr;
    fprintf(f, "%f%%", computation_perc);
    fprintf(f, "        %f%%", memory_perc);
    fprintf(f, "    %f%%\n", branch_perc);
    float branch_taken_perc = (float)state->b_taken_count / (float)state->branch_count;
    float branch_not_taken_perc = (float)state->b_not_taken_count / (float)state->branch_count;
    fprintf(f, "\nBranch Taken     Branch Not Taken\n");
    fprintf(f, "%f%%", branch_taken_perc);
    fprintf(f, "        %f%%\n", branch_not_taken_perc);
}

void write_to_output(struct arm_state *state, int index){
    FILE *f;
    const char *title;
    if(index == 1){
        f = fopen("sum_array.txt", "w");
        title = "Result for sum array test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 2){
        f = fopen("find_max.txt", "w");
        title = "Result for find max test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 3){
        f = fopen("fibo_iter.txt", "w");
        title = "Result for fibo iteration test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 4){
        f = fopen("fibo_rec.txt", "w");
        title = "Result for fibo recursion test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 5){
        f = fopen("find_sub_in_s.txt", "w");
        title = "Result for find sub in s test:";
        fprintf(f, "%s\n", title);
    }
    fprintf(f, "Total instructions: %d\n", (char *)(state->exec_instr_count));
    fprintf(f, "Total computation instructions: %d\n", (char *)(state->compu_count));
    fprintf(f, "Total memory instructions: %d\n", (char *)(state->mem_count));
    fprintf(f, "Total branch instructions: %d\n", (char *)(state->branch_count));
    fprintf(f, "Total branch taken: %d\n", (char *)(state->b_taken_count));
    fprintf(f, "Total branch not taken: %d\n", (char *)(state->b_not_taken_count));
    write_read_regs_to_file(state, f);
    write_written_regs_to_file(state, f);
    fprintf(f, "armemu_total_time_usecs = %f us\n", state->armemu_total_time_usecs);  
    fprintf(f, "native_total_time_usecs = %f us\n", state->native_total_time_usecs);
    int times = state->armemu_total_time_usecs / state->native_total_time_usecs;
    fprintf(f, "native method is: %d times faster than armemu method\n", times);
    write_inst_percentage(f, state);
    fclose(f);
}

/*test part*/
void sum_array_test(struct arm_state *state, unsigned int *func, int *p_array, int size){
    printf("Start sum array test and print input array:\n");
    print_array_c(p_array, size);
    init_arm_state(state, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
    int sum;
    sum = armemu(state);
    printf("Sum is: %d\n", sum);
    gettime_array(state, (unsigned int *) func, p_array, size, 1);
    print_analysis(state);
    write_to_output(state, 1);
}                  

void find_max_test(struct arm_state *state, unsigned int *func, int *p_array, int size){
    printf("Start max array test and print input array:\n");
    print_array_c(p_array, size);
    init_arm_state(state, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
    int max;
    max = armemu(state);
    printf("Max is: %d\n", max);
    gettime_array(state, (unsigned int *) func, p_array, size, 2);
    print_analysis(state);
    write_to_output(state, 2);
}

void fibo_iter_test(struct arm_state *state, unsigned int *func, int size){
    printf("Start iteration fibonacci test:\n");
    init_arm_state(state, (unsigned int *) func, size, 0, 0, 0);
    int fibo_iter;
    fibo_iter = armemu(state);
    printf("Fibo iteration result for %d's element is: %d\n", size, fibo_iter);
    gettime_fibo(state, (unsigned int *) func, size, 1);
    print_analysis(state);
    write_to_output(state, 3);
}

void fibo_rec_test(struct arm_state *state, unsigned int *func, int size){
    printf("Start recursion fibonacci test:\n");
    init_arm_state(state, (unsigned int *) func, size, 0, 0, 0);
    int fibo_rec;
    fibo_rec = armemu(state);
    printf("Fibo recursion result for %d's element is: %d\n", size, fibo_rec);
    gettime_fibo(state, (unsigned int *) func, size, 2);
    print_analysis(state);
    write_to_output(state, 4);
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
    init_arm_state(state, (unsigned int *) func, int_p_s, int_p_sub, s_len, s_sub_len);
    pos = armemu(state);
    printf("Start position is: %d\n", pos);
    gettime_find_s_in_sub(state, (unsigned int *) func, int_p_s, int_p_sub, s_len, s_sub_len);
    print_analysis(state);
    write_to_output(state, 5);
}

void run_test(struct arm_state *state, int *p_array, char *p_s, char *p_sub, int size){
    sum_array_test(state, (unsigned int *) sum_array_s, p_array, size);
    find_max_test(state, (unsigned int *) find_max_s, p_array, size);
    fibo_iter_test(state, (unsigned int *) fibo_iter_s, size);
    fibo_rec_test(state, (unsigned int *) fibo_rec_s, size);
    find_sub_in_s_test(state, (unsigned int *) find_sub_in_s_s, p_s, p_sub);
}

/*main part*/
int main(int argc, char **argv){
    struct arm_state state;
    int *p_array = state.array;
    int size = 20;
    init_array_c(p_array, size);
    strcpy(state.s, "hello");
    strcpy(state.sub, "llo");
    char *p_s = state.s;
    char *p_sub = state.sub;

    run_test(&state, p_array, p_s, p_sub, size);
  
    return 0;
}



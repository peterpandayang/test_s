#include <stdbool.h>
#include <stdio.h>
#include <string.h>



#define NREGS 16
#define STACK_SIZE 1024
#define SP 13
#define LR 14
#define PC 15
#define VALUE_MAX_STR_LEN 64


int sum_array_s(int *p, int n);
int find_max_s(int *p, int n);
int fibo_iter_s(int n);
int fibo_rec_s(int n);

struct arm_state {
    unsigned int regs[NREGS];
    unsigned int cpsr;
    unsigned char stack[STACK_SIZE];
};

struct value_st {
    int pos_array[VALUE_MAX_STR_LEN];
    char s[50];
    char sub1[10];
};

void init_arm_state(struct arm_state *as, unsigned int *func, unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3){
    int i;

    /* zero out all arm state */
    for (i = 0; i < NREGS; i++) {
        as->regs[i] = 0;
    }

    as->cpsr = 0;

    for (i = 0; i < STACK_SIZE; i++) {
        as->stack[i] = 0;
    }

    as->regs[PC] = (unsigned int) func;
    as->regs[SP] = (unsigned int) &as->stack[STACK_SIZE];
    as->regs[LR] = 0;

    as->regs[0] = arg0;
    as->regs[1] = arg1;
    as->regs[2] = arg2;
    as->regs[3] = arg3;
}

void init_array_c(int *p_pos, int n){
    int i = 0;
    for(i = 0; i < n ; i++){
        p_pos[i] = i;
    }
    p_pos[i] = '\0';
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
    unsigned int rd, rn, rm, imme;

    iw = *((unsigned int *) state->regs[PC]);    
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;

    if(is_imme_dp(iw)){
        imme = iw & 0xFF;
        state->regs[rd] = state->regs[rn] + imme;
    }
    else{
        rm = iw & 0xF;
        state->regs[rd] = state->regs[rn] + state->regs[rm];
    }

    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
    }
}

void armemu_sub(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm, imme;

    iw = *((unsigned int *) state->regs[PC]);    
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;

    if(is_imme_dp(iw)){
        imme = iw & 0xFF;
        state->regs[rd] = state->regs[rn] - imme;
    }
    else{
        rm = iw & 0xF;
        state->regs[rd] = state->regs[rn] - state->regs[rm];
    }

    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
    }
}


bool is_cmp_inst(unsigned int iw){
    unsigned int opcode;
    opcode = (iw >> 21) & 0b1111;
    return (opcode == 0b1010);
}

void armemu_cmp(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm, imme;

    iw = *((unsigned int *) state->regs[PC]);    
    rn = (iw >> 16) & 0xF;
    state->cpsr = 0;

    if(is_imme_dp(iw)){
        imme = iw & 0xFF;
        if(state->regs[rn] - imme < 0){
            state->cpsr = 0x80000000;
        }
        if(state->regs[rn] - imme == 0){
            state->cpsr = 0x40000000;
        }
    }
    else{
        rm = iw & 0xF;
        if(state->regs[rn] - state->regs[rm] < 0){
            state->cpsr = 0x80000000;
        }
        if(state->regs[rn] - state->regs[rm] == 0){
            state->cpsr = 0x40000000;
        }
    }
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
    }

    if (rd != PC) {
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
            state->regs[rd] = *((unsigned int *)state->regs[rn] + offset);
        }        
    }

    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
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
            *((unsigned int *)state->regs[rn] + offset) = state->regs[rd];
        }        
    }

    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
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
    rn = iw & 0b1111;

    state->regs[PC] = state->regs[rn];
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
        state->regs[PC] = state->regs[PC] + 8 + offset * 4;
    }
    else if(is_beq_inst(iw)){
        if(state->cpsr == 0x40000000){
            state->regs[PC] = state->regs[PC] + 8 + offset * 4;
        }
        else{
            state->regs[PC] = state->regs[PC] + 4;
        }
    }
    else if(is_bge_inst(iw)){
        if(state->cpsr >> 31 == 0b0){
            state->regs[PC] = state->regs[PC] + 8 + offset * 4;
        }
        else{
            state->regs[PC] = state->regs[PC] + 4;
        }
    }
    else if(is_bne_inst(iw)){
        if(state->cpsr >> 30 == 0b0){
            state->regs[PC] = state->regs[PC] + 8 + offset * 4;
        }
        else{
            state->regs[PC] = state->regs[PC] + 4;
        }
    }
    else if(is_b_default_inst(iw)){
        state->regs[PC] = state->regs[PC] + 8 + offset * 4;
    }
    state->cpsr = 0;
}


/*armemu part*/
void armemu_one(struct arm_state *state){
    unsigned int iw;
    
    iw = *((unsigned int *) state->regs[PC]);

    if (is_bx_inst(iw)) {
        armemu_bx(state);
    } 
    else if (is_b_inst(iw)) {
        armemu_b(state);
    } 
    else if(is_data_pro_inst(iw)){
        armemu_data_pro(state);
    }
    else if(is_mem_inst(iw)){
        armemu_mem(state);
    }
}

unsigned int armemu(struct arm_state *state){

    while (state->regs[PC] != 0) {
        armemu_one(state);
    }

    return state->regs[0];
}


/*test part*/
void sum_array_test(struct arm_state *as, unsigned int *func, int *p_array, int size){
    init_arm_state(as, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
    int sum;
    sum = armemu(as);
    printf("sum is: %d\n", sum);
}                  

void find_max_test(struct arm_state *as, unsigned int *func, int *p_array, int size){
    init_arm_state(as, (unsigned int *) func, (unsigned int) p_array, size, 0, 0);
    int max;
    max = armemu(as);
    printf("max is: %d\n", max);
}

void fibo_iter_test(struct arm_state *as, unsigned int *func, int size){
    init_arm_state(as, (unsigned int *) func, size, 0, 0, 0);
    int fibo_iter;
    fibo_iter = armemu(as);
    printf("fibo iteration result is: %d\n", fibo_iter);
}

void fibo_rec_test(struct arm_state *as, unsigned int *func, int size){
    init_arm_state(as, (unsigned int *) func, size, 0, 0, 0);
    int fibo_rec;
    fibo_rec = armemu(as);
    printf("fibo recursion result is: %d\n", fibo_rec);
}


/*main part*/
int main(int argc, char **argv){
    struct arm_state state;
    struct value_st v_st;
    int *p_pos_array = v_st.pos_array;
    int size = 20;

    init_array_c(p_pos_array, size);

    sum_array_test(&state, (unsigned int *) sum_array_s, p_pos_array, size);
    find_max_test(&state, (unsigned int *) find_max_s, p_pos_array, size);
    fibo_iter_test(&state, (unsigned int *) fibo_iter_s, size);
    fibo_rec_test(&state, (unsigned int *) fibo_rec_s, 2);
  
    return 0;
}







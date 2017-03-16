#include <stdbool.h>
#include <stdio.h>
#include <string.h>



#define NREGS 16
#define STACK_SIZE 1024
#define SP 13
#define LR 14
#define PC 15
#define VALUE_MAX_STR_LEN 64


int add(int a, int b);

int sum_array_s(int *p, int n);

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

bool is_add_inst(unsigned int iw){
    unsigned int op;
    unsigned int opcode;

    op = (iw >> 26) & 0b11;
    opcode = (iw >> 21) & 0b1111;

    return (op == 0) && (opcode == 0b0100);
}

void armemu_add(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm;

    iw = *((unsigned int *) state->regs[PC]);
    
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;
    rm = iw & 0xF;

    state->regs[rd] = state->regs[rn] + state->regs[rm];
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
    }
}

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

void armemu_one(struct arm_state *state){
    unsigned int iw;
    
    iw = *((unsigned int *) state->regs[PC]);

    if (is_bx_inst(iw)) {
        armemu_bx(state);
    } else if (is_add_inst(iw)) {
        armemu_add(state);
    }
}


unsigned int armemu(struct arm_state *state){

    while (state->regs[PC] != 0) {
        armemu_one(state);
    }

    return state->regs[0];
}

void sum_array_test(struct arm_state *as, unsigned int *func, int *p_array, int size){
    init_arm_state(as, (unsigned int *) func, p_array, size, 0, 0);
}                  
    
int main(int argc, char **argv)
{
    struct arm_state state;
    unsigned int r;
    struct value_st v_st;
    int *p_pos_array = v_st.pos_array;
    
    init_arm_state(&state, (unsigned int *) add, 1, 2, 0, 0);
    r = armemu(&state);
    printf("r = %d\n", r);

    sum_array_test(&state, (unsigned int *) sum_array_s, v_st.pos_array, 20);
  
    return 0;
}

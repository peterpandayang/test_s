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


/* data processing part*/
bool is_add_inst(unsigned int iw){
    unsigned int opcode;
    opcode = (iw >> 21) & 0b1111;
    return (opcode == 0b0100);
}

void armemu_add(struct arm_state *state){
    unsigned int iw;
    unsigned int rd, rn, rm, imme;

    iw = *((unsigned int *) state->regs[PC]);    
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;

    if(((iw >> 25) & 0b1) == 0b1){
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

    if(((iw >> 25) & 0b1) == 0b1){
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

    if(((iw >> 25) & 0b1) == 0b1){
        unsigned int imme = iw & 0xFF;
        printf("move: %d\n", imme);
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
    else if(is_cmp_inst(iw)){
        armemu_cmp(state);
    }
}


/*memory part*/
bool is_mem_inst(unsigned int iw){
    unsigned int op;
    op = (iw >> 26) & 0b01;
    return op == 0b01;
}

void armemu_mem(struct arm_state *state){

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

void armemu_b(struct arm_state *state){
    unsigned int iw, imme;

    iw = *((unsigned int *) state->regs[PC]);
    imme = 0xFFFFFF - (iw & 0xFFFFFF) - 1;
    printf("immd is: %d\n", imme);

    if((iw >> 28 & 0b1111) == 0b0000){
        if(state->cpsr == 0x40000000){
            state->regs[PC] = state->regs[PC] + 8 + imme * 4;
        }
        else{
            printf("lalala\n");
            state->regs[PC] = state->regs[PC] + 4;
        }
    }
    else if((iw >> 28 & 0b0000) == 0b1110){
        printf("hehehe\n");
        state->regs[PC] = state->regs[PC] + 8 + imme * 4;
    }
    state->regs[PC] = 0;
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
    int max;
    max = armemu(as);
    printf("max is: %d\n", max);
}                  
    
/*main part*/
int main(int argc, char **argv)
{
    struct arm_state state;
    struct value_st v_st;
    int *p_pos_array = v_st.pos_array;

    sum_array_test(&state, (unsigned int *) sum_array_s, v_st.pos_array, 20);
  
    return 0;
}







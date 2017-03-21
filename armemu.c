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
int find_sub_in_s_s(char *s, char *sub);

struct arm_state {
    unsigned int regs[NREGS];
    unsigned int cpsr;
    unsigned char stack[STACK_SIZE];
};

struct value_st {
    int pos_array[VALUE_MAX_STR_LEN];
    char s[50];
    char sub[10];
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
        }        
    }

    if (rd != PC) {
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
            state->regs[rd] = *((unsigned int *)(state->regs[rn] + offset));
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
            *((unsigned int *)(state->regs[rn] + offset)) = state->regs[rd];
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

void save_link_addr(struct arm_state *state){
    state->regs[LR] = state->regs[PC] + 4;
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
        if(is_beq_inst(iw)){
            if(state->cpsr == 0x40000000){
                state->regs[PC] = state->regs[PC] + 8 + offset * 4;
            }
            else{
                state->regs[PC] = state->regs[PC] + 4;
            }
        }
        else{
            save_link_addr(state);
            state->regs[PC] = state->regs[PC] + 8 + offset * 4;
        }
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

void find_sub_in_s_test(struct arm_state *as, unsigned int *func, char *p_s, char *p_sub){
    unsigned int int_p_s = (unsigned int)((unsigned int *)p_s);
    unsigned int int_p_sub = (unsigned int)((unsigned int *)p_sub);
    printf("inner address is: %d\n", p_s);
    printf("char is: %c\n", *p_s);
    init_arm_state(as, (unsigned int *) func, int_p_s, int_p_sub, 0, 0);
    int pos;
    pos = armemu(as);
    printf("position is: %d\n", pos);
    // printf("char is: %c\n", pos);
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


int strstr_test(char *p_s, char *p_sub){
    printf("Test for strstr: \n");
    int pos; 
    printf("Larger string: ");
    print_str(p_s);
    printf("Smaller string: ");
    print_str(p_sub);
    printf("Test for strstr in C: \n");

    pos = find_sub_in_s_c(p_s, p_sub);
    if(pos != -1){
        printf("Find substring at:\n");
        print_str(p_s);
        print_arrow(pos);
    }
    else{
        printf("Can't find substring\n");
    }

    // printf("Test for strstr in Assembly: \n");
    // pos = 0;
    // pos = find_sub_in_s_s(p_s, p_sub);
    // if(pos != -1){
    //     printf("Find substring at:\n");
    //     print_str(p_s);
    //     print_arrow(pos);
    // }
    // else{
    //     printf("Can't find substring\n");
    // }
    // printf("\n");
    return 0;
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

int print_arrow(int pos){
    int i;
    for(i = 0; i < pos; i++){
        printf(" ");
    }
    printf(" ^\n");
    return 0;
}

/*main part*/
int main(int argc, char **argv){
    struct arm_state state;
    struct value_st v_st;
    int *p_pos_array = v_st.pos_array;
    int size = 20;

    init_array_c(p_pos_array, size);
    strcpy(v_st.s, "hello");
    strcpy(v_st.sub, "llo");
    char *p_s = v_st.s;
    char *p_sub = v_st.sub;

    // sum_array_test(&state, (unsigned int *) sum_array_s, p_pos_array, size);
    // find_max_test(&state, (unsigned int *) find_max_s, p_pos_array, size);
    // fibo_iter_test(&state, (unsigned int *) fibo_iter_s, size);
    // fibo_rec_test(&state, (unsigned int *) fibo_rec_s, size);


    strstr_test(p_s, p_sub);
    // printf("address is: %d\n", p_s);
    // find_sub_in_s_test(&state, (unsigned int *) find_sub_in_s_s, p_s, p_sub);
  
    return 0;
}







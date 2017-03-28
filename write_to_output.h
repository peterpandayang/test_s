#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#ifndef WRITE_TO_OUTPUT_H_INCLUCED
#define WRITE_TO_OUTPUT_H_INCLUCED

void write_to_output(struct arm_state *state, int index){
    FILE *f;
    const char *title;
    if(index == 1){
        f = fopen("sum_array_pos.txt", "w");
        title = "Result for sum positive array test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 2){
        f = fopen("sum_array_neg.txt", "w");
        title = "Result for sum negetive array test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 3){
        f = fopen("sum_array_zero.txt", "w");
        title = "Result for sum zero array test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 4){
        f = fopen("sum_array_large.txt", "w");
        title = "Result for large array test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 5){
        f = fopen("find_max_pos.txt", "w");
        title = "Result for find max test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 6){
        f = fopen("find_max_neg.txt", "w");
        title = "Result for find max test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 7){
        f = fopen("find_max_zero.txt", "w");
        title = "Result for find max test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 8){
        f = fopen("find_max_large.txt", "w");
        title = "Result for find max test:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 10){
        f = fopen("fibo_iter_ten.txt", "w");
        title = "Result for fibo iteration test of 10:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 19){
        f = fopen("fibo_iter_nineteen.txt", "w");
        title = "Result for fibo iteration test of 19:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 11){
        f = fopen("fibo_rec_ten.txt", "w");
        title = "Result for fibo recursion test of 10:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 20){
        f = fopen("fibo_rec_nineteen.txt", "w");
        title = "Result for fibo recursion test of 19:";
        fprintf(f, "%s\n", title);
    }
    else if(index == 9){
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

#endif
.global find_sub_in_s_s
.func find_sub_in_s_s

find_sub_in_s_s: 
	sub sp, sp, #64   
	sub r12, r2, r3
	str r12, [sp, #48]
	str r3, [sp, #56]	
	str r0, [sp]
	mov r2, #0
	str r1, [sp, #8]
	str r2, [sp, #16]
	str lr, [sp, #64]

loop: 
	ldr r1, [sp, #48]  
	ldr r2, [sp, #16]
	sub r2, r2, #1
	cmp r2, r1
	beq not_found 
	ldr r0, [sp]
	add r2, r2, #1
	add r0, r0, r2
	ldr r1, [sp, #8]
	ldrb r3, [r0]    
	ldrb r12, [r1]   
	cmp r3, r12
	bleq check_common 
	ldr lr, [sp, #64]
	cmp r0, #0
	beq found
	ldr r2, [sp, #16]
	add r2, r2, #1
	str r2, [sp, #16]
	b loop

found:
	ldr r0, [sp, #16]
	bx lr

not_found:
	mov r0, #0
	sub r0, r0, #1
	bx lr

.endfunc


.func check_common

check_common: 

	mov r2, #0 
	str r0, [sp, #24]
	str r1, [sp, #32]
	str r2, [sp, #40]

check_common_loop:    
	ldr r1, [sp, #56]
	ldr r2, [sp, #40]
	cmp r2, r1
	beq has_common
	ldr r0, [sp, #24]
	add r0, r0, r2
	ldr r1, [sp, #32]
	add r1, r1, r2
	ldrb r3, [r0]
	ldrb r12, [r1]   
	cmp r3, r12
	bne no_common 
	add r2, r2, #1
	str r2, [sp, #40]
	b check_common_loop

no_common:   
	mov r0, #0
	sub r0, r0, #1
	bx lr

has_common:
	mov r0, #0
	bx lr

.endfunc

.global find_sub_in_s_s
.func find_sub_in_s_s

find_sub_in_s_s:
	mov r2, #0
	sub sp, sp, #56
	str r0, [sp]
	str r1, [sp, #8]
	str r2, [sp, #16]


	cmp r2, #0
	beq out


	bl get_len


	cmp r0, #8
	beq out


	str r0, [sp, #48]
	ldr r0, [sp, #8]
	bl get_len
	str r0, [sp, #56]
	ldr r1, [sp, #48]
	sub r2, r1, r0
	str r2, [sp, #48]

loop:
	ldr r1, [sp, #48]
	ldr r2, [sp, #16]
	sub r2, r2, #1
	cmp r2, r1
	beq not_found
	ldr r3, [sp]
	add r2, r2, #1
	add r3, r3, r2
	ldr r12, [sp, #8]
	cmp r3, r12
	mov r0, r3
	mov r1, r12
	cmp r3, r12
	bl check_common
	cmp r0, #0
	beq found
	ldr r2, [sp, #16]
	add r2, r2, #1
	str r2, [sp, #16]
	b loop

found:
	ldr lr, [sp, #16]
	bx lr

not_found:
	mov lr, #-1
	bx lr

out:
	mov lr, r0
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
	ldr r3, [sp, #24]
	add r3, r3, r2
	ldr r12, [sp, #32]
	add r12, r12, r2
	cmp r3, r12
	bne no_common
	add r2, r2, #1
	str r2, [sp, #40]
	b check_common_loop

no_common:
	mov lr, #-1
	bx lr

has_common:
	mov lr, #0
	bx lr

.endfunc


.func get_len

get_len:
	mov r2, #0

get_len_loop:
	add r1, r0, r2
	cmp r1, #0
	bne done
	add r2, r2, #8
	b get_len_loop

done:
	mov lr, r2
	bx lr
	
.endfunc





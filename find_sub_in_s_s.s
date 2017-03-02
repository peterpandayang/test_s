.global find_sub_in_s_s
.func find_sub_in_s_s

find_sub_in_s_s:
	mov r2, #0
	sub sp, sp, #56
	str r0, [sp]
	str r1, [sp, #8]
	str r2, [sp, #16]

	bl get_len

	cmp r2, #0
	beq out

	cmp r2, #1
	beq out

	cmp r0, #0
	b out
	cmp r0, #1
	b out
	cmp r0, #2
	beq out
	cmp r0, #3
	beq out
	cmp r0, #4
	beq out
	cmp r0, #5
	beq out
	cmp r0, #6
	beq out
	cmp r0, #6
	beq out
	cmp r0, #7
	beq out
	cmp r0, #8
	beq out
	cmp r0, #9
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
	ldr r0, [sp, #16]
	bx lr

not_found:
	mov r0, #-1
	bx lr

out:
	mov r0, #10
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
	mov r0, #-1
	bx lr

has_common:
	mov r0, #0
	bx lr

.endfunc


.func get_len

get_len:
	mov r2, #0

get_len_loop:
	add r3, r0, r2
	cmp r2, #0
	beq done
	/*ldr r1, [r0]*/
	mov r1, #0
	cmp r1, #0
	beq done
	add r2, r2, #1
	b get_len_loop

done:
	mov r0, #1
	bx lr
	
.endfunc






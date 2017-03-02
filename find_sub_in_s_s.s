.global find_sub_in_s_s
.func find_sub_in_s_s

find_sub_in_s_s:
	mov r2, #0
	sub sp, sp, #64
	str r0, [sp]
	str r1, [sp, #8]
	str r2, [sp, #16]
	str lr, [sp, #64]
	bl get_len

	cmp r0, #2
	beq out

	ldr lr, [sp, #64]
	str r0, [sp, #48]
	ldr r0, [sp, #8]
	str lr, [sp, #64]
	bl get_len
	ldr lr, [sp, #64]
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
	ldrb r1, [r3]
	cmp r1, #0
	beq done
	add r2, r2, #1
	b get_len_loop

done:
	mov r0, r2
	bx lr
	
.endfunc






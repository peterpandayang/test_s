.globl fibo_rec_s
.fibo_rec_s

fibo_rec_s:
	sub sp, sp, #8
	str lr, [sp]

	cmp r0, #0
	bne not_0
	b fibo_s_end

not_0:
	cmp r0, #1
	bne rec
	b fibo_s_end

rec:
	str r0, [sp, #4]
	sub r0, r0, #1
	bl fibo_s
	mov r1, r0
	ldr r0, [sp, #4]
	sub r0, r0, #2
	sub sp, sp, #8
	bl fibo_s
	add sp, sp, #8
	mov r2, r0
	add r0, r1, r2


fibo_s_end:
	ldr lr, [sp]
	add sp, sp, #8
	bx lr
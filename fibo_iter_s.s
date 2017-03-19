.globl fibo_iter_s
.func fibo_iter_s

fibo_iter_s:

	sub sp, sp, #16
	str r0, [sp, #16]
	mov r0, #0
	str r0, [sp, #12]
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #8]
	str r0, [sp, #4]

loop:
	ldr r3, [sp, #12]
	ldr r12, [sp, #16]
	cmp r3, r12
	beq done
	ldr r0, [sp]
	ldr r1, [sp, #4]
	ldr r2, [sp, #8]
	str r1, [sp]
	str r2, [sp, #4]
	add r2, r2, r1
	str r2, [sp, #8]
	add r3, r3, #1
	str r3, [sp, #12]
	b loop

done:
	ldr r0, [sp]
	bx lr



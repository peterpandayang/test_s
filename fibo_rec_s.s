.globl fibo_rec_s
.func fibo_rec_s

fibo_rec_s:
	mov r5, r0
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
	bl fibo_rec_s
	sub sp, sp, #4
	str r0, [sp]
	add sp, sp, #4
	ldr r0, [sp, #4]
	mov r0, r0

	sub r0, r0, #2
	sub sp, sp, #8
	bl fibo_rec_s
	add sp, sp, #8
	sub sp, sp, #4
	ldr r1, [sp]
	add sp, sp, #4
	add r0, r0, r1

fibo_s_end:
	ldr lr, [sp]
	add sp, sp, #8
	bx lr
	
	

# 	sub sp, sp, #8
# 	str lr, [sp]

# 	cmp r0, #0
# 	bne not_0
# 	b fibo_s_end

# not_0:
# 	cmp r0, #1
# 	bne rec
# 	b fibo_s_end

# rec:
# 	str r0, [sp, #4]
# 	sub r0, r0, #1
# 	bl fibo_rec_s
# 	str r0, [sp, #-4]
# 	ldr r0, [sp, #4]
# 	sub r0, r0, #2
# 	sub sp, sp, #8
# 	bl fibo_rec_s
# 	add sp, sp, #8
# 	ldr r1, [sp, #-4]
# 	add r0, r0, r1


# fibo_s_end:
# 	ldr lr, [sp]
# 	add sp, sp, #8
# 	bx lr

.global sum_array_s
.func sum_array_s

sum_array_s:
	mov r2, #0
	mov r3, #0
	
loop:
	cmp r2, r1
	beq done
	ldr r12, [r0]
	add r3, r3, r12
	add r0, r0, #4
	add r2, r2, #1
	b loop

done:
	mov r0, r3
	bx lr


.global find_max_s
.func find_max_s

find_max_s:
	mov r2, #0
	mov r3, #0

loop:
	cmp r2, r1
	beq done
	ldr r4, [r0]
	cmp r4, r3
	bge update
	add r0, r0, #4
	add r2, r2, #1
	b loop

update:
	mov r3, r4
	add r0, r0, #4
	add r2, r2, #1
	b loop

done:
	mov r0, r3
	bx lr




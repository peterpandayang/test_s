.func main
main:
		mov sp, #128
		add sp, sp, sp
		add sp, sp, sp
		add sp, sp, sp
		add sp, sp, sp
		add sp, sp, sp
		add sp, sp, sp
        add sp, sp, sp
        sub sp, sp, #4
        mov r0, #1
        str r0, [sp]
        sub sp, sp, #4
        mov r0, #2
        str r0, [sp]
        sub sp, sp, #4
        mov r0, #3
        str r0, [sp]
        sub sp, sp, #4
        mov r0, #4
        str r0, [sp]
        sub sp, sp, #4
        mov r0, #5
        str r0, [sp]        
        mov r0, sp
        mov r1, #5
        bl add
end:
        b end
.endfunc

.func add

add:
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
.endfunc


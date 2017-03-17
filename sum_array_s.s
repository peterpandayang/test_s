.global sum_array_s
.func sum_array_s

sum_array_s:
	# mov r2, #0
	# mov r3, #0


	# add r0, r1, #1
	mov r0, #1
	cmp r0, #2
	# beq end
	b end2

end:
	mov r0, #3
	bx lr

end2:
	mov r0, #2
	bx lr

# loop:
# 	cmp r2, r1
# 	beq done
# 	ldr r4, [r0]
# 	add r3, r3, r4
# 	add r0, r0, #4
# 	add r2, r2, #1
# 	b loop

# done:
# 	mov r0, r3
# 	bx lr


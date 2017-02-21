/* -- ifelse.s */
.global ifelse
.func ifelse

/* C code
x = 99;
y = 99;

if (x == y) {
	x = x + 1;
} else {
	y = y + 2;
}

*/
	
ifelse:
	
	cmp r0, r1
	bne else
	add r0, r0, #1
	b endif
else:	
	add r1, r1, #3
	mov r0, r1
endif:
	bx lr      /* Return from main */



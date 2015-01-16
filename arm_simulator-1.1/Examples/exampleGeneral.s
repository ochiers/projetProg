.global main
.text

main:
	bl init
	
test:
	add r1, r8		@r1 = r1 + r8 = 9
	sub r6, r2 		@r6 = r6 - r2 = 4 
	subs r0, r8 	@ZNCV change
	str r8, [r13]
	ldr r9, [r13] 	@r9 = 8
	blx r12
init:
	mov r0, #0
	mov r1, #1
	mov r2, #2
	mov r3, #3
	mov r4, #4
	mov r5, #5
	mov r6, #6
	mov r7, #7
	mov r8, #8
	mov r12, #0x6c
	mov r13, #0x1000
	bl test
end:
    swi 0x123456
.data

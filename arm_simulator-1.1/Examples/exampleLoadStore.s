.global main
.text
main:
    
init:
	mov r0, #0x12
	mov r1, #0x34
	add r0, r1, r0, lsl #8
	mov r1, #0x56
	add r0, r1, r0, lsl #8
	mov r1, #0x78
	add r0, r1, r0, lsl #8
	mov r1, #0x87
	mov r2, #0x65
	add r1, r2, r1, lsl #8
	mov r2, #0x43
	mov r3, #0x21
	mov r4, #0xFF
	
testldr:
	str r3, [r4]
	ldr r5, [r4]
	
testldrb:
	strb r0, [r4]
	ldrb r6, [r4]
	
testldrh:
	strh r1, [r4]
	ldrh r7, [r4]
	
testpreIncrement:
	mov r4, #0xFB
	ldr r8, [r4, #+4]

testpostIncrement:
	mov r4, #0xFF
	ldr r9, [r4], #+4
	
testldrm:
	mov r2, #0x43
	add r1, r2, r1, lsl #8
	mov r2, #0x21
	add r1, r2, r1, lsl #8
	mov r2, #0x11
	mov r3, #0x22
	add r2, r3, r2, lsl #8
	mov r3, #0x33
	add r2, r3, r2, lsl #8
	mov r3, #0x44
	add r2, r3, r2, lsl #8
	mov r4, #0x80
	stm r4, {r0, r1, r2}
	ldm r4, {r10, r11, r12}

fin:
    swi 0x123456
	
adr_valeur: .word valeur

.data
valeur: .word 1


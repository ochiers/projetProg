.global main
.text
main:
    bl init

fin:
    swi 0x123456
    
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
	bl fin
	
	
adr_valeur: .word valeur

.data
valeur: .word 1


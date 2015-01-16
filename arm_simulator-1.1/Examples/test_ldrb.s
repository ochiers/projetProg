.data


val_plus_10_b: .byte 10

.text

main:
	ldr r0, adr_0_b
	
	ldrb r1, [r0,#-2]			@r1=254
	ldrsb r2,  [r0,#-2]			@r2=-2
	
	mov r14, #4
	ldrb r3, [r0,r14]			@r3=4
	ldrsb r4, [r0,r14]			@r4=4
	
	ldrb r5, [r0,#-8]!			@r5=248	r0=0x1000-8=0xFF8
	mov r14, #8
	ldr r0, adr_0_b
	ldrsb r6, [r0,r14]!			@r6=8		r0=0x1008

	ldrsb r7, [r0], #1			@r7=8		r0=0x100A
	ldrsb r8, [r0], -r14		@r8=10		r0=0x1002

fin:
    swi 0x123456



adr_moins_10_b: .word val_moins_10_b	
adr_moins_9_b: .word val_moins_9_b 	
adr_moins_8_b: .word val_moins_8_b 	
adr_moins_7_b: .word val_moins_7_b 	
adr_moins_6_b: .word val_moins_6_b 	
adr_moins_5_b: .word val_moins_5_b 	
adr_moins_4_b: .word val_moins_4_b 	
adr_moins_3_b: .word val_moins_3_b 	
adr_moins_2_b: .word val_moins_2_b 	
adr_moins_1_b: .word val_moins_1_b	
adr_0_b: .word val_0_b	
adr_plus_1_b: .word val_plus_1_b 
adr_plus_2_b: .word val_plus_2_b 
adr_plus_3_b: .word val_plus_3_b 
adr_plus_4_b: .word val_plus_4_b 
adr_plus_5_b: .word val_plus_5_b 
adr_plus_6_b: .word val_plus_6_b 
adr_plus_7_b: .word val_plus_7_b 
adr_plus_8_b: .word val_plus_8_b 
adr_plus_9_b: .word val_plus_9_b 
adr_plus_10_b: .word val_plus_10_b	

.org 0xFD6

val_moins_10_b: .byte -10	
val_moins_9_b: .byte -9	
val_moins_8_b: .byte -8	
val_moins_7_b: .byte -7	
val_moins_6_b: .byte -6	
val_moins_5_b: .byte -5	
val_moins_4_b: .byte -4	
val_moins_3_b: .byte -3	
val_moins_2_b: .byte -2	
val_moins_1_b: .byte -1	
val_0_b: .byte 0	
val_plus_1_b: .byte 1
val_plus_2_b: .byte 2
val_plus_3_b: .byte 3
val_plus_4_b: .byte 4
val_plus_5_b: .byte 5
val_plus_6_b: .byte 6
val_plus_7_b: .byte 7
val_plus_8_b: .byte 8
val_plus_9_b: .byte 9


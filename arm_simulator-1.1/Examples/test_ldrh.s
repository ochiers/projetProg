.data

.text

main:
	ldr r0, adr_0_hw
	
	ldrh r1, [r0,#-2]			@r1=65535
	ldrsh r2,  [r0,#-2]			@r2=-1
	
	mov r14, #4
	ldrh r3, [r0,r14]			@r3=2
	ldrsh r4, [r0,r14]			@r4=2
	
	ldrh r5, [r0,#-8]!			@r5=65532	r0=0x1000-8=0xFF8
	mov r14, #8
	ldr r0, adr_0_hw
	ldrsh r6, [r0,r14]!			@r6=4		r0=0x1008

	ldrsh r7, [r0], #2			@r7=4		r0=0x100A
	ldrsh r8, [r0], -r14		@r8=5		r0=0x1002

fin:
    swi 0x123456


adr_moins_10_hw: .word val_moins_10_hw
adr_moins_9_hw: .word val_moins_9_hw	
adr_moins_8_hw: .word val_moins_8_hw	
adr_moins_7_hw: .word val_moins_7_hw	
adr_moins_6_hw: .word val_moins_6_hw	
adr_moins_5_hw: .word val_moins_5_hw	
adr_moins_4_hw: .word val_moins_4_hw	
adr_moins_3_hw: .word val_moins_3_hw	
adr_moins_2_hw: .word val_moins_2_hw	
adr_moins_1_hw: .word val_moins_1_hw	
adr_0_hw: 		.word val_0_hw	
adr_plus_1_hw: .word  val_plus_1_hw
adr_plus_2_hw: .word  val_plus_2_hw
adr_plus_3_hw: .word  val_plus_3_hw
adr_plus_4_hw: .word  val_plus_4_hw
adr_plus_5_hw: .word  val_plus_5_hw
adr_plus_6_hw: .word  val_plus_6_hw
adr_plus_7_hw: .word  val_plus_7_hw
adr_plus_8_hw: .word  val_plus_8_hw
adr_plus_9_hw: .word  val_plus_9_hw
adr_plus_10_hw: .word val_plus_10_hw


.org 0xFCC
.balign 2
val_moins_10_hw: .hword -10	
val_moins_9_hw: .hword -9	
val_moins_8_hw: .hword -8	
val_moins_7_hw: .hword -7	
val_moins_6_hw: .hword -6	
val_moins_5_hw: .hword -5	
val_moins_4_hw: .hword -4	
val_moins_3_hw: .hword -3	
val_moins_2_hw: .hword -2	
val_moins_1_hw: .hword -1	
val_0_hw: .hword 0				@0x1000
val_plus_1_hw: .hword 1
val_plus_2_hw: .hword 2
val_plus_3_hw: .hword 3
val_plus_4_hw: .hword 4
val_plus_5_hw: .hword 5
val_plus_6_hw: .hword 6
val_plus_7_hw: .hword 7
val_plus_8_hw: .hword 8
val_plus_9_hw: .hword 9
val_plus_10_hw: .hword 10

.data

.text

main:
	ldr r0, adr_0_w

	@R0 non modifié ( 1 2 et 3)	
	ldr r1, [r0,#-8]					@r1=-2
	ldr r2, [r0,#16]					@r2=4
	
	ldr r3, [r0,+r2]					@r3=1
	ldr r4, [r0,-r2]					@r4=-1


	mov r14, #1
	ldr r5, [r0,r14,lsl #5]			@r5=mem[adr_0_w+(1<<5)]=mem[adr_0_w+32]=+8
	
	mov r14, #0x60
	ldr r6, [r0,r14,lsr #2]			@r6=mem[adr_0_w+(0x60>>2)]=mem[adr_0_w+24]=+6
	
	mvn r14, #7						@r14= -8
	ldr r7, [r0,-r14,asr #1]		@r7=mem[adr_0_w-(-8>>1]=mem[adr_0_w+4]=+1
	
	mov r14, #1
	ldr r8, [r0,r14,ror #30]		@r5=mem[adr_0_w+4]=+1	
	
	movs r14, #8					@r14=2*4 et Cflag=0
	ldr r9, [r0,-r14,rrx]			@r9=mem[adr_0_w-4]=-1

	
@Pre-indexed	
	ldr r1, [r0,#-40]!				@r1=-10 et r0=0xFD8
	add r1, r1, #2
	ldr r2, [r0,-r1]!				@r2=0 et r0=FE0
	
	movs r14, #40					@r14=2*10 et Cflag=0
	ldr r3, [r0,r14,rrx]!			@r3=mem[0xff8+20]=-3	r0=ff8+20=ff4		

@Post-indexed
	ldr r0, adr_0_w
	ldr r4, [r0], #-40				@r4=0 et r0=adr_0_w-40
	
	mov r14, #0
	mov r13, #-4
	ldr r5, [r0], -r13				@r5=-10 r0=adr_0_w-40-(-4)= adr_0_w-36=4060
	
	mov r14, #0
	mov r13, #4
	ldr r6, [r0], r13, LSL #2		@r5=-9 r0=4060+(4<<2=16)=4076


fin:
    swi 0x123456


adr_moins_10_w: .word val_moins_10_w	
adr_moins_9_w: .word val_moins_9_w 	
adr_moins_8_w: .word val_moins_8_w 	
adr_moins_7_w: .word val_moins_7_w 	
adr_moins_6_w: .word val_moins_6_w 	
adr_moins_5_w: .word val_moins_5_w 	
adr_moins_4_w: .word val_moins_4_w 	
adr_moins_3_w: .word val_moins_3_w 	
adr_moins_2_w: .word val_moins_2_w 	
adr_moins_1_w: .word val_moins_1_w 	
adr_0_w:       .word val_0_w
adr_plus_1_w:  .word val_plus_1_w
adr_plus_2_w:  .word val_plus_2_w
adr_plus_3_w:  .word val_plus_3_w
adr_plus_4_w:  .word val_plus_4_w
adr_plus_5_w:  .word val_plus_5_w
adr_plus_6_w:  .word val_plus_6_w
adr_plus_7_w:  .word val_plus_7_w
adr_plus_8_w:  .word val_plus_8_w
adr_plus_9_w:  .word val_plus_9_w
adr_plus_10_w: .word val_plus_10_w


.org 0xFB8
val_moins_10_w: .word -10	
val_moins_9_w: .word -9	
val_moins_8_w: .word -8	
val_moins_7_w: .word -7	
val_moins_6_w: .word -6	
val_moins_5_w: .word -5	
val_moins_4_w: .word -4	
val_moins_3_w: .word -3	
val_moins_2_w: .word -2	
val_moins_1_w: .word -1	
val_0_w: .word 0			@0x1000
val_plus_1_w: .word 1
val_plus_2_w: .word 2
val_plus_3_w: .word 3
val_plus_4_w: .word 4
val_plus_5_w: .word 5
val_plus_6_w: .word 6
val_plus_7_w: .word 7
val_plus_8_w: .word 8
val_plus_9_w: .word 9
val_plus_10_w: .word 10

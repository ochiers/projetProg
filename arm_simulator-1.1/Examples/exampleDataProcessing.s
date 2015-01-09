.global main
.text
main:
	mov r0, #0
	mov r1, #1
	mov r2, #2
	mov r3, #3
	mov r4, #4
	mov r5, #5
	mov r6, #6
	mov r7, #7
	mov r8, #8
	
	add	r0, #5
	add r0, r1
	add r0, r0, #8
	add r0, r0, r1
	add r0, r1, r3, lsl #2
	add r0, r1, r3, lsl r4
	add r0, r1, r3, lsr #2
	add r0, r1, r3, lsr r4
	add r0, r1, r3, asr #2
	add r0, r1, r3, asr r4
	add r0, r1, r3, ror #2
	add r0, r1, r3, ror r4
	add r0, r1, r3, rrx

	and r0, r1
	and r0, r1, r3, asr #2
	
	eor r0, r1, r3, ror r4
	eor r0, r0, #8
	
	sub r0, r1, r3, lsr r4
	sub r0, r1
			
	adc r0, r1, r3, lsl #2
	adc r0, #7
	
	sbc r0, r1, r3, rrx
	sbc r0, r0, r1
	
	rsc r0, r1
	rsc r0, r1, r3, asr r4
		
	/*tst r0,r1,r3,ror r4*/
	tst r0, #7 
	
	/*teq r0, r1, r3, ror #2*/
	teq r0, r1
	
	cmp r0, r1
	/*cmp r0, r1, r3, rrx*/
	
	/*cmn r0, r1, r3, lsr #2*/
	cmn r0, r1
	
	orr r0, #7
	orr r0, r1, r3, lsl #2
	
	/*mov r0, r1, r3, ror #2
	mov r0, r0, #8*/
	
	bic r0, r0, #8
	bic r0, r1, r3, asr #2
	
	/*mvn r0, r0, r1
	mvn r0, r1, r3, ror r4*/
	
	swi 0x123456

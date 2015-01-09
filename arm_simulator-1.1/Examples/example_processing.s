.global main
.text
main:
	add	r0, #7 
	add r0, r1
	add r0, r0, #8
	add r0, r0, r1
	add r0, r1, r3, lsl #8
	add r0, r1, r3, lsl r4
	add r0, r1, r3, lsr #8
	add r0, r1, r3, lsr r4
	add r0, r1, r3, asr #8
	add r0, r1, r3, asr r4
	add r0, r1, r3, ror #8
	add r0, r1, r3, ror r4
	add r0, r1, r3, rrx

	and r0, r1
	and r0, r1, r3, asr #8
	
	eor r0, r1, r3, ror r4
	eor r0, r0, #8
	
	sub r0, r1, r3, lsr r4
	sub r0, r1
			
	adc r0, r1, r3, lsl #8
	adc r0, #7
	
	sbc r0, r1, r3, rrx
	sbc r0, r0, r1
	
	rsc r0, r1
	rsc r0, r1, r3, asr r4
		
	/*tst r0,r1,r3,ror r4*/
	tst r0, #7 
	
	/*teq r0, r1, r3, ror #8*/
	teq r0, r1
	
	cmp r0, r1
	/*cmp r0, r1, r3, rrx*/
	
	/*cmn r0, r1, r3, lsr #8*/
	cmn r0, r1
	
	orr r0, #7
	orr r0, r1, r3, lsl #8
	
	/*mov r0, r1, r3, ror #8
	mov r0, r0, #8*/
	
	bic r0, r0, #8
	bic r0, r1, r3, asr #8
	
	/*mvn r0, r0, r1
	mvn r0, r1, r3, ror r4*/
	
	swi 0x123456

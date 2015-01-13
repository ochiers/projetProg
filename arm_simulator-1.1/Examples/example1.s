.global main
.text
decr:
    subs r0, r0, #1
    mov pc, lr

main:
    mov r0, #5
loop:
    bl decr
	mrs r1, CPSR
	
	orr r1, #0xF0000000
	
	msr CPSR, r1
	
	msr CPSR, 0x00000010
	
    bne loop
end:
    swi 0x123456
.data

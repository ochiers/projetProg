.global main
.text

main:
    mov r0, #5
    mov r1, #6
    mov r4, #4
    mov r5, #5
    subs r4, r5
    mrs r0, CPSR
    mrs r1, SPSR
    
	mov r1, #0xF0000000
	msr CPSR, r1
	msr CPSR, #0x00000010
end:
    swi 0x123456
.data

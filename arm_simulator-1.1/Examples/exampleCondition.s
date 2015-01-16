.global main
.text

decr:
    sub r1, #1
    mov pc, lr
exit:
    swi 0x123456
main:
    mov r0, #0
    mov r1, #1
    cmp r0, r1
    blne decr
    cmp r0, r1
    blne decr
    beq exit
.data

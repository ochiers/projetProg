.global main
.text
main:
    mov r0, #0x12
    mov r7, #0x12
    mov r9, #0x34
    add r7, r9, r7, lsl #8
    mov r9, #0x56
    add r7, r9, r7, lsl #8
    mov r9, #0x78
    add r7, r9, r7, lsl #8
    mov r9, #0x55
    mov r12, #0x87
    add r9, r12, r9, lsl #8
    mov r6, #0x80
    mov r10, #0x81
    str r7, [r6]
    mov r6, #0x80
    strb r9, [r10]
    ldrb r8, [r6]
    mov r1, #0x34
    add r0, r1, r0, lsl #8
    mov r1, #0x56
    add r0, r1, r0, lsl #8
    mov r1, #0x78
    add r0, r1, r0, lsl #8
    mov r1, #0x2000
    str r0, [r1]
    ldrb r2, [r1]
    add r1, r1, #3
    ldrb r3, [r1]
    swi 0x123456

adr_valeur: .word valeur

.data
valeur: .word 1


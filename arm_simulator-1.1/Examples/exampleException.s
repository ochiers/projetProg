

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

	mov r0, r1
	add r0, #5

	swi 0x123456



@@ ----------------------------------------
@@ Declaration des traitants d'interruption
@@ ----------------------------------------
handler_irq:			mov r0, #123
				subs pc, lr, #4		@ place egalement SPSR dans CPSR
handler_undefined_instruction:	mov r4, #6
				subs pc, lr, #4		@ place egalement SPSR dans CPSR
handler_swi:			mov r4, #7
				subs pc, lr, #4		@ place egalement SPSR dans CPSR
handler_prefetch_abort:		mov r4, #8
				subs pc, lr, #4		@ place egalement SPSR dans CPSR
handler_data_abort:		mov r4, #9
				subs pc, lr, #4		@ place egalement SPSR dans CPSR
handler_fiq:			mov r7, #10
				subs pc, lr, #4		@ place egalement SPSR dans CPSR




@@ ----------------------------------------
@@ Declaration des vecteurs d'interruption
@@ Devraient etre a l'adresse 0x0
@@ ----------------------------------------
.org 0xFE0
_reset			: b main				@0xFE0
_undefined_instruction	: b handler_undefined_instruction	@0xFE4
_swi			: b handler_swi				@0xFE8
_prefetch_abort		: b handler_prefetch_abort		@0xFEC
_data_abort		: b handler_data_abort			@0xFF0
_irq			: b handler_irq				@0xFF4
_fiq			: b handler_fiq



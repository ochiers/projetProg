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

	subs r0, #5
	subs r0, r1
	subs r0, r0, #8
	subs r0, r0, r1
	subs r0, r1, r3, lsl #2
	subs r0, r1, r3, lsl r4
	subs r0, r1, r3, lsr #2
	subs r0, r1, r3, lsr r4
	subs r0, r1, r3, asr #2
	subs r0, r1, r3, asr r4
	subs r0, r1, r3, ror #2
	subs r0, r1, r3, ror r4
	subs r0, r1, r3, rrx



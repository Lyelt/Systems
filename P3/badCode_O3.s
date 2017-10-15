	.arch armv6
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"badCode.c"
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu vfp
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	sub	sp, sp, #20
	ldr	r6, .L3
	ldr	r7, .L3+4
	ldr	r5, .L3+8
	mov	r2, r6
	mov	r3, r7
	mov	r0, r5
	bl	printf
	mov	r0, r5
	mvn	r1, #0
	bl	printf
	mov	r4, #0
	mvn	r1, #0
	mov	r0, #8
	str	r0, [sp, #8]
	mov	r2, r6
	mov	r3, r7
	stm	sp, {r1, r4}
	ldr	r0, .L3+12
	bl	printf
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L4:
	.align	2
.L3:
	.word	1909375720
	.word	1071959848
	.word	.LC0
	.word	.LC1
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"p= %g\012\000"
	.space	1
.LC1:
	.ascii	"k=%u  m=%li\012\000"
	.ident	"GCC: (Raspbian 6.3.0-18+rpi1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",%progbits

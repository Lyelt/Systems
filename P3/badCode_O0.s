	.arch armv6
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"badCode.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"p= %g\012\000"
	.align	2
.LC1:
	.ascii	"k=%u  m=%li\012\000"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu vfp
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #40
	str	r0, [fp, #-24]
	str	r1, [fp, #-28]
	ldr	r2, .L3
	ldr	r3, .L3+4
	strd	r2, [fp, #-12]
	mvn	r3, #0
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-16]
	uxth	r3, r3
	add	r3, r3, #1
	uxth	r3, r3
	strh	r3, [fp, #-18]	@ movhi
	mov	r3, #8
	strh	r3, [fp, #-20]	@ movhi
	ldrd	r2, [fp, #-12]
	ldr	r0, .L3+8
	bl	printf
	ldr	r1, [fp, #-16]
	ldr	r0, .L3+8
	bl	printf
	ldrsh	r3, [fp, #-18]
	ldrsh	r2, [fp, #-20]
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	ldr	r3, [fp, #-16]
	str	r3, [sp]
	ldrd	r2, [fp, #-12]
	ldr	r0, .L3+12
	bl	printf
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
.L4:
	.align	2
.L3:
	.word	1909375720
	.word	1071959848
	.word	.LC0
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (Raspbian 6.3.0-18+rpi1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",%progbits

	.arch armv8-a
	.file	"race.c"
	.text
	.global	counter
	.bss
	.align	2
	.type	counter, %object
	.size	counter, 4
counter:
	.zero	4
	.text
	.align	2
	.global	increment
	.type	increment, %function
increment:
.LFB0:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	str	x0, [sp, 8]
	str	wzr, [sp, 28]
	b	.L2
.L3:
	adrp	x0, counter
	add	x0, x0, :lo12:counter
	ldr	w0, [x0]
	add	w1, w0, 1
	adrp	x0, counter
	add	x0, x0, :lo12:counter
	str	w1, [x0]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L2:
	ldr	w1, [sp, 28]
	mov	w0, 16959
	movk	w0, 0xf, lsl 16
	cmp	w1, w0
	ble	.L3
	mov	x0, 0
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	increment, .-increment
	.section	.rodata
	.align	3
.LC0:
	.string	"Final counter = %d (expected %d)\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB1:
	.cfi_startproc
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	stp	x29, x30, [sp, 32]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 32
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 24]
	mov	x1, 0
	add	x4, sp, 8
	mov	x3, 0
	adrp	x0, increment
	add	x2, x0, :lo12:increment
	mov	x1, 0
	mov	x0, x4
	bl	pthread_create
	add	x4, sp, 16
	mov	x3, 0
	adrp	x0, increment
	add	x2, x0, :lo12:increment
	mov	x1, 0
	mov	x0, x4
	bl	pthread_create
	ldr	x0, [sp, 8]
	mov	x1, 0
	bl	pthread_join
	ldr	x0, [sp, 16]
	mov	x1, 0
	bl	pthread_join
	adrp	x0, counter
	add	x0, x0, :lo12:counter
	ldr	w0, [x0]
	mov	w2, 33920
	movk	w2, 0x1e, lsl 16
	mov	w1, w0
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	mov	w0, 0
	mov	w1, w0
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x3, [sp, 24]
	ldr	x2, [x0]
	subs	x3, x3, x2
	mov	x2, 0
	beq	.L7
	bl	__stack_chk_fail
.L7:
	mov	w0, w1
	ldp	x29, x30, [sp, 32]
	add	sp, sp, 48
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits

	.file	"arming.c"
	.arch msp430g2553
	.cpu 430
	.mpy none

	.section	.init9,"ax",@progbits
	.p2align 1,0
.global	main
	.type	main,@function
/***********************
 * Function `main' 
 ***********************/
main:
	mov	r1, r4
	add	#2, r4
	mov	#23168, &__WDTCTL
	mov.b	#0, &__P1SEL
	mov.b	#0, &__P2SEL
	call	#setup
.L2:
	call	#loop
	jmp	.L2
.LIRD0:
.Lfe1:
	.size	main,.Lfe1-main
;; End of function 

	.comm state,2,2
	.text
	.p2align 1,0
.global	setup
	.type	setup,@function
/***********************
 * Function `setup' 
 ***********************/
setup:
	push	r4
	mov	r1, r4
	add	#2, r4
	mov	#0, &state
	mov.b	&__P1DIR, r15
	bis.b	#2, r15
	mov.b	r15, &__P1DIR
	mov.b	&__P1DIR, r15
	bis.b	#4, r15
	mov.b	r15, &__P1DIR
	mov.b	&__P1DIR, r15
	and.b	#llo(-9), r15
	mov.b	r15, &__P1DIR
	mov.b	&__P1DIR, r15
	and.b	#llo(-17), r15
	mov.b	r15, &__P1DIR
	mov.b	&__P1DIR, r15
	bis.b	#1, r15
	mov.b	r15, &__P1DIR
	mov.b	&__P1OUT, r15
	and.b	#llo(-3), r15
	mov.b	r15, &__P1OUT
	mov.b	&__P1OUT, r15
	and.b	#llo(-5), r15
	mov.b	r15, &__P1OUT
	mov.b	&__P1OUT, r15
	and.b	#llo(-9), r15
	mov.b	r15, &__P1OUT
	mov.b	&__P1OUT, r15
	and.b	#llo(-17), r15
	mov.b	r15, &__P1OUT
	mov.b	&__P1OUT, r15
	and.b	#llo(-2), r15
	mov.b	r15, &__P1OUT
	pop	r4
	ret
.Lfe2:
	.size	setup,.Lfe2-setup
;; End of function 

	.p2align 1,0
.global	loop
	.type	loop,@function
/***********************
 * Function `loop' 
 ***********************/
loop:
	push	r4
	mov	r1, r4
	add	#2, r4
	sub	#4, r1
	mov	&state, r15
	cmp	#1, r15
	jeq	.L7
	cmp	#2, r15
	jge	.L10
	cmp	#0, r15
	jeq	.L6
	jmp	.L5
.L10:
	cmp	#2, r15
	jeq	.L8
	cmp	#3, r15
	jeq	.L9
	jmp	.L5
.L6:
	mov	#1, r15
	call	#blink
	mov	#0, r12
	mov	#1, r13
	mov	#1, r14
	mov	#1, r15
	call	#sequence
	mov	r15, &state
	jmp	.L4
.L7:
	mov	#2, r15
	call	#blink
	mov	#1, r12
	mov	#2, r13
	mov	#1, r14
	mov	#0, r15
	call	#sequence
	mov	r15, &state
	jmp	.L4
.L8:
	mov	#3, r15
	call	#blink
	mov	#2, r12
	mov	#3, r13
	mov	#0, r14
	mov	#1, r15
	call	#sequence
	mov	r15, &state
	jmp	.L4
.L9:
	mov	#4, r15
	call	#blink
	mov.b	&__P1OUT, r15
	bis.b	#2, r15
	mov.b	r15, &__P1OUT
	mov	#0, -6(r4)
	jmp	.L12
.L15:
	mov	#0, -4(r4)
	jmp	.L13
.L14:
	mov.b	&__P2OUT, r15
	xor.b	#llo(-128), r15
	mov.b	r15, &__P2OUT
	add	#1, -4(r4)
.L13:
	cmp	#100, -4(r4)
	jl	.L14
	add	#1, -6(r4)
.L12:
	cmp	#500, -6(r4)
	jl	.L15
	mov.b	&__P1OUT, r15
	and.b	#llo(-3), r15
	mov.b	r15, &__P1OUT
	jmp	.L4
.L5:
	nop
.L4:
	add	#4, r1
	pop	r4
	ret
.Lfe3:
	.size	loop,.Lfe3-loop
;; End of function 

	.p2align 1,0
.global	sequence
	.type	sequence,@function
/***********************
 * Function `sequence' 
 ***********************/
sequence:
	push	r4
	mov	r1, r4
	add	#2, r4
	sub	#8, r1
	mov	r15, -10(r4)
	mov	r14, -8(r4)
	mov	r13, -6(r4)
	mov	r12, -4(r4)
	mov.b	&__P1IN, r15
	mov.b	r15, r15
	mov	r15, r14
	and	#8, r14
	mov	#1, r15
	cmp	#0, r14
	jne	.L17
	mov	#0, r15
.L17:
	cmp	-10(r4), r15
	jne	.L18
	mov.b	&__P1IN, r15
	mov.b	r15, r15
	mov	r15, r14
	and	#16, r14
	mov	#1, r15
	cmp	#0, r14
	jne	.L19
	mov	#0, r15
.L19:
	cmp	-8(r4), r15
	jne	.L18
	mov	-6(r4), r15
	jmp	.L20
.L18:
	mov	-4(r4), r15
.L20:
	add	#8, r1
	pop	r4
	ret
.Lfe4:
	.size	sequence,.Lfe4-sequence
;; End of function 

	.p2align 1,0
.global	blink
	.type	blink,@function
/***********************
 * Function `blink' 
 ***********************/
blink:
	push	r4
	mov	r1, r4
	add	#2, r4
	add	#llo(-16), r1
	mov	r15, -4(r4)
	mov	#0, -6(r4)
	jmp	.L22
.L36:
	mov.b	&__P1OUT, r15
	bis.b	#1, r15
	mov.b	r15, &__P1OUT
	mov	#0, -18(r4)
	jmp	.L23
.L26:
	mov	#0, -16(r4)
	jmp	.L24
.L25:
	mov.b	&__P2OUT, r15
	xor.b	#llo(-128), r15
	mov.b	r15, &__P2OUT
	add	#1, -16(r4)
.L24:
	cmp	#100, -16(r4)
	jl	.L25
	add	#1, -18(r4)
.L23:
	cmp	#80, -18(r4)
	jl	.L26
	mov.b	&__P1OUT, r15
	and.b	#llo(-2), r15
	mov.b	r15, &__P1OUT
	mov	#0, -14(r4)
	jmp	.L27
.L30:
	mov	#0, -12(r4)
	jmp	.L28
.L29:
	mov.b	&__P2OUT, r15
	xor.b	#llo(-128), r15
	mov.b	r15, &__P2OUT
	add	#1, -12(r4)
.L28:
	cmp	#100, -12(r4)
	jl	.L29
	add	#1, -14(r4)
.L27:
	cmp	#80, -14(r4)
	jl	.L30
	mov	-6(r4), r15
	add	#llo(-1), r15
	cmp	-4(r4), r15
	jne	.L31
	mov	#0, -10(r4)
	jmp	.L32
.L35:
	mov	#0, -8(r4)
	jmp	.L33
.L34:
	mov.b	&__P2OUT, r15
	xor.b	#llo(-128), r15
	mov.b	r15, &__P2OUT
	add	#1, -8(r4)
.L33:
	cmp	#100, -8(r4)
	jl	.L34
	add	#1, -10(r4)
.L32:
	cmp	#10000, -10(r4)
	jl	.L35
.L31:
	add	#1, -4(r4)
.L22:
	cmp	-4(r4), -6(r4)
	jl	.L36
	mov	-4(r4), r15
	add	#16, r1
	pop	r4
	ret
.Lfe5:
	.size	blink,.Lfe5-blink
;; End of function 


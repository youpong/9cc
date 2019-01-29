	.intel_syntax noprefix
	.globl	fibonacci
fibonacci:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8

	# store args in stack
	mov	[rsp], rdi

	# if (n == 0)
	cmp	rdi, 0
	jne	L0

	# return 0
	mov	rax, 0
	jmp	L9
L0:
	# if (n == 1)
	cmp	rdi, 1
	jne	L1

	# return 1
	mov	rax, 1
	jmp	L9
L1:
	# return fibonacci(n-2) + fibonacci(n-1)
	mov	rdi, rbp
	sub	rdi, 8
	mov	rdi, [rdi]
	sub	rdi, 2
	call 	fibonacci
	push	rax

	mov	rsi, rbp
	sub	rsi, 8
	mov	rsi, [rsi]
	sub	rsi, 1
	mov 	rdi, rsi
	call 	fibonacci

	pop	rdi
	add	rax, rdi
L9:	
	mov	rsp, rbp
	pop	rbp
	ret
	

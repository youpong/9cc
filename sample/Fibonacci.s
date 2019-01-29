	.intel_syntax noprefix
	.globl	fibonacci
fibonacci:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8

	# store args in stack
	mov	[rbp-8], rdi

	# if (n == 0)
	cmp	rdi, 0
	jne	L0

	# return 0
	mov	rax, 0

	mov	rsp, rbp
	pop	rbp
	ret
L0:
	# if (n == 1)
	cmp	rdi, 1
	jne	L1

	# return 1
	mov	rax, 1

	mov	rsp, rbp
	pop	rbp
	ret
L1:
	# return fibonacci(n-2) + fibonacci(n-1)

	sub	rdi, 2
	call 	fibonacci
	push	rax

	mov	rdi, [rbp-8]
	sub	rdi, 1
	call 	fibonacci

	pop	rdi
	add	rax, rdi

	mov	rsp, rbp
	pop	rbp
	ret
	

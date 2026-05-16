default abs
section .text 

scan:
	push rbp
		
	mov rax, 0
	mov rdi, 0        ; stdin дескриптор
	mov rsi, buff_num ; буффер для числа
	mov rdx, 8  ; максимальное количество байт
	
	syscall 

	call make_num_from_buff	

	pop rbp
	ret

make_num_from_buff:	
	xor rbx, rbx
	
	convert_num:
		lodsb
		cmp rax, 0ah
		jz convert_end
		
		push rax
		mov rax, rbx
		mov rbx, 10
		mul rbx
		mov rbx, rax
		pop rax

		add rbx, rax
		sub rbx, '0'

		jmp convert_num
	
	convert_end:

	mov rax, rbx
	ret

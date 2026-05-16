PRINT_BUFF_SIZE equ 128
default abs
section .text 

; global _start
; global print

; _start:
; 	push 1589
; 	call print
; 	add rsp, 8
;
; 	mov rax, 0x3c
; 	xor rdx, rdx
; 	syscall


;===================================================================================
; Notes: print для стандартной библеотеки
;===================================================================================

print:
	push rbp
	; push rbx

	xor r10, r10

	mov rbp, rsp
	add rbp, 16 ; тут надо посмотреть
	;add rbp, 8 
	lea rdi, [ buff_print]

; ==== Тут должен быть цикл с принтом числа, пока что будет принт одного числа
	call print_num_dec 		

	exit:
	mov rax, `\n`
	stosb
	lea r13, [ buff_print]
	sub rdi, r13
	mov rdx, rdi

	mov rax, 1
	mov rdi, 1
	lea rsi, [ buff_print]
	syscall 
	
	; pop rbx		
	pop rbp
	
	ret
;======================================================================================================
; Regs: rbp - хранит адресс начала аргументов (пока что только на один), поэтому его не трогать!!!
; 	rdi - адресс на buff_print 
;======================================================================================================

print_num_dec:
	push rsi
	push rcx
	
	mov rax, [rbp + 8 * 0]

	test rax, 1000000000000000000000000000000b ; 2 ^ 31; 32 бит
	jz plus 
	
	push rax
	mov rax, '-'
	stosb
	call check_print_buff	
	pop rax
	
	not eax 	
	inc eax
	plus:
	push rdi
	lea rdi, [ buff_num] 
	mov rsi, rdi	
	mov rcx, 10

	.converting_num:
		xor rdx, rdx
		div rcx			
		push rax
		mov rax, rdx
		call get_asci_code_reg
		pop rax
		test rax, rax
		jnz .converting_num
	
	sub rdi, rsi
	mov rdx, rdi
	pop rdi

	call make_buff_rev

	pop rcx
	pop rsi
	ret

check_print_buff:
	push rax
	push rdx
	push rsi
	push rcx

	lea r13, [ end_of_buff_print]
	cmp rdi, r13

	jb .end_func
		mov rax, 1
		mov rdi, 1
		mov rdx, PRINT_BUFF_SIZE
		lea rsi, [ buff_print]
		syscall
	
	lea rdi, [ buff_print]
	
	.end_func:	
	pop rcx
	pop rsi
	pop rdx
	pop rax	
	
	ret

get_asci_code_reg:
	cmp al, 10
	jge .letter
		add al, '0'
		stosb
		ret

	.letter:
		lea rax, ['A' + rax - 10]
		stosb
		ret

make_buff_rev:
	push rcx
	add rdi, rdx

	lea r13, [ end_of_buff_print]
	cmp rdi, r13
	jb no_flush
		call check_print_buff
		add rdi, rdx
	no_flush:
  
	push rdi
	dec rdi 
	mov rcx, rdx	
	
	.byte_cpy:
		cld
		lodsb
		std
		stosb
		cld
		call check_print_buff
		loop .byte_cpy	

	pop rdi
	pop rcx
	ret

section .data

buff_num times 8 dq 0
buff_rev times 8 dq 0

buff_print times 128 db 0
end_of_buff_print:

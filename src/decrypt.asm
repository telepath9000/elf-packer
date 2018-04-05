%macro pushx 1-*
	%rep %0
		push %1
		%rotate 1
	%endrep
%endmacro

%macro popx 1-*
	%rep %0
		%rotate -1
		pop %1
	%endrep
%endmacro

[BITS 64]

section .text

global		decrypt:function
global		load_size:data
global		data_size:data

load_size	dq		end - decrypt
data_size	dq		end - data_start

decrypt:	
			pushfq
			pushx	rax, rdi, rsi, rsp, rdx, rcx

			mov		rdi, 1
			lea		rsi, [rel msg]
			mov		rdx, msg_len
			mov		rax, rdi
			syscall

			jmp		unpack

msg			db		"...WOODY...", 10, 0
msg_len		equ		$ - msg

unpack:

			; call mprotect on text section
			mov		rdi, [rel addr]
			mov		rsi, [rel size]
			mov		rdx, 7
			mov		rax, 10
			syscall

			mov		rax, [rel addr]
			mov		rcx, [rel size]
			mov		rdx, [rel key]

			add		rcx, rax

loop:
			xor		byte [rax], dl
			ror		rdx, 8
			inc		rax
			cmp		rax, rcx
			jnz		loop
			jmp		r12

data_start:
key:		dq		0x9999999999999999
addr:		dq		0xAAAAAAAAAAAAAAAA
size:		dq		0xBBBBBBBBBBBBBBBB
new_entry	dq		0xCCCCCCCCCCCCCCCC

end:

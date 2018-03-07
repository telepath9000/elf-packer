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

decrypt:
			call	next
next:		pop		r8
			pushfq
			pushx	rax, rdi, rsi, rsp, rdx, rcx

;			mov		rdi, 1
;			lea		rsi, [rel msg]
;			mov		rdx, msg_len
;			mov		rax, rdi
;			syscall

			jmp		unpack

msg			db		"...WOODY...", 10, 0
msg_len		equ		$ - msg

unpack:
		;;; replace the code ouy deleted here 

loop:
			xor		byte [rax], dl
			ror		rdx, 8
			inc		rax
			cmp		rax, rcx
			mov		rdi, 1
			jnz		loop
			popx	rax, rdi, rsi, rsp, rdx, rcx
			jmp		r9

data_start:
sign:		dq		0x8888888888888888
key:		dq		0x9999999999999999
addr:		dq		0xAAAAAAAAAAAAAAAA
size:		dq		0xBBBBBBBBBBBBBBBB
load_size	dq		end - decrypt
data_size	dq		end - data_start

end:

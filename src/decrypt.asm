		call	st
st:
		pop		r12

[BITS 64]

global		decrypt:function
global		load_size:data
global		data_size:data

load_size	dq		end - decrypt
data_size	dq		end - data_start

decrypt:
			mov		r11,[rel new_entry]
			mov		r8,	[rel addr]
			cmp		r11, r8
			mov		r9, [rel addr_diff]
			jae		sub_addr
			jb		add_addr
sub_addr:
			sub		r12, r9
			jmp		cont
add_addr:
			add		r12, r9
cont:
			mov		rdi, 1
			lea		rsi, [rel msg]
			mov		rdx, msg_len
			mov		rax, rdi
			syscall

			jmp		unpack

msg			db		"...IwuzHEre...", 10, 0
msg_len		equ		$ - msg

unpack:

			; call mprotect on text section
			mov		rdi, r12
			mov		rsi, [rel size]
			mov		rdx, 7
			mov		rax, 10
			syscall

			mov		rax, r12
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
addr_diff	dq		0xDDDDDDDDDDDDDDDD

end:

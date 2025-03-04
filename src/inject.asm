file_begin:

[BITS 64]

global		inject:function
global		load_size:data
global		data_size:data

load_size	dq	end_file - file_begin
data_size	dq	end_file - data_start

inject:
			mov		rdi, 1
			lea		rsi, [rel msg]
			mov		rdx, msg_len
			mov		rax, rdi
			syscall

			jmp		cont

msg			db	"[Decrypting...]", 10, 0
msg_len		equ	$ - msg

cont:

			mov		rax, [rel addr]
			mov		rcx, [rel file_size]
			mov		rdx, [rel key]

			add		rcx, rax

	.loop	xor		byte[rax], dl
			ror		rdx, 8
			inc		rax
			cmp		rax, rcx
			jnz		.loop

			mov		rax, [rel addr]
			jmp		rax

data_start:
key:			dq	0x9999999999999999
addr:			dq	0xAAAAAAAAAAAAAAAA
file_size:		dq	0xBBBBBBBBBBBBBBBB

end_file:
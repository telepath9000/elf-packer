; convert to 64 bity support

global encrypt

section .text

encrypt:
			pushad
			lea		esi, [esp + 32 + 4]
			lodsd
			xchg	ecx, eax
			lodsd
			xchg	edi, eax
			lodsd
			xchg	eax, esi
			push	esi

			lodsd
			xchg	eax, ebp
			lodsd
			xchg	eax, ebx
			lodsd
			xchg	eax, edx
			lodsd
			xchg	eax, ebp

			xor		eax, [edi]
			xor		ebx, [edi + 4]
			xor		edx, [edi + 8]
			xor		ebp, [edi + 12]
			test	ecx, ecx
			mov		cl, 16

enc:
			pushfd
			jz		dec
			add		eax, ebx
			rol		ebx, 5
			xor		ebx, eax
			rol		eax, 16
			add		edx, ebp
			rol		ebp, 8
			xor		ebp, edx
			add		eax, ebp
			rol		ebp, 13
			xor		ebp, eax
			add		edx, ebx
			ror		ebx, 7
			xor		ebx, edx
			ror		ebx, 16
			jmp		check

dec:
			ror		edx, 16
			xor		ebx, edx
			ror		ebx, 7
			sub		edx, ebx
			xor		ebp, eax
			ror		ebp, 13
			sub		eax, ebp
			xor		ebp, edx
			ror		ebp, 8
			sub		edx, ebp
			ror		eax, 16
			xor		ebx, eax
			ror		ebx, 5
			sub		eax, ebx

check:
			popfd
			loop	enc

post:
			xor		eax, [edi]
			xor		ebx, [edi + 4]
			xor		edx, [edi + 8]
			xor		ebp, [edi + 12]
			pop		edi
			stosd
			xchg	eax, ebx
			stosd
			xchg	eax, edx
			stosd
			xchg	eax, ebp
			stosd
			popad
			ret

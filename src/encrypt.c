#include "../include/packer.h"

uint64_t		gen_key(void)
{	
	return ((((uint64_t) rand() <<  0) & 0x000000000000FFFFull) | 
			(((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) | 
			(((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
			(((uint64_t) rand() << 48) & 0xFFFF000000000000ull));
}

static uint64_t	rotate_key(uint64_t key)
{
	return ((key << 8) | (key >> 56));
}

int			encrypt_section(t_elf *bin)
{
	uint64_t	key_tmp;
	uint64_t	*end;

	key_tmp = bin->enc_key;
	end = (uint64_t *)(bin->file_ptr + bin->encrypt_off + bin->section_size);
	for (uint64_t *p = (uint64_t *)(bin->file_ptr + bin->encrypt_off); p < end; p++) {
		*p ^= key_tmp;
		key_tmp = rotate_key(key_tmp);
	}
	return 1;
}

#include "../include/woody.h"

uint64_t	get_random_key(void)
{
	uint64_t	key;

	syscall(__NR_getrandom, &key, 8, 0);
	printf("key: 0x%lX\n", key);
	return (key);
}

uint64_t	rotate_right(uint64_t key)
{
	return (((key & 0xFF) << 56) | (key >> 8));
}

void		encrypt_section(char *ptr, t_woody *elf)
{
	char		*text;
	uint64_t	tmp_key;

	elf->key = get_random_key();
	text = ptr + elf->text_off;
	for (size_t i = 0; i < elf->text_size; i++)
	{
		text[i] ^= (char)tmp_key;
		tmp_key = rotate_right(tmp_key);
	}
}

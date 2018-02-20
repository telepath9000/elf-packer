#ifndef WOODY_H
# define WOODY_H

# include <stdio.h>
# include <fcntl.h>
# include <elf.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/syscall.h>
# include <stdint.h>
# include <sys/mman.h>

# define APPEND_SHELLCODE 0
# define CAVE_FOUND 1

extern uint64_t	load_size;
extern uint64_t	data_size;

int				g_flag;

typedef struct	s_woody
{
	uint64_t	key;
	uint64_t	base_addr;
	uint64_t	inject_size;
	uint64_t	text_size;
	size_t		sh_strtbl_size;
	size_t		file_size;
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr	*shdr;
	uint32_t	new_entry;
	uint32_t	base_off;
	uint32_t	text_addr;
	uint32_t	text_off;
	char		*load;
	char		*sh_stroff;
}				t_woody;

extern void		decrypt(void);
void			encrypt(int enc, void *key, void *buf);

#endif

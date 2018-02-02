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

# define SEC ".init", ".text", ".fini", ".rodata", ".eh_frame_hdr",
# define SEC2 ".eh_frame", ".init_array", ".fini_array", ".jrc",
# define SEC3 ".dynamic", ".got.plt", ".data"
# define SEC_SIZE 12

typedef struct	s_offs
{
	uint32_t	size;
	uint32_t	offset;
	uint32_t	addr;
	int64_t		relative_off;
	char		*name;
}				t_offs;

typedef struct	s_woody
{
	size_t		inject_size;
	size_t		sh_strtbl_size;
	size_t		file_size;
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr	*shdr;
	t_offs		*sh_offsets;
	uint32_t	base_addr;
	uint32_t	new_entry;
	char		*inject;
	char		*sh_stroff;
	uint8_t		num_offs;
}				t_woody;

void	encrypt(int enc, void *key, void *buf);

#endif

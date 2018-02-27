#define _GNU_SOURCE
#ifndef WOODY_H
# define WOODY_H

# include <stdio.h>
# include <elf.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/syscall.h>
# include <unistd.h>

# define APPEND_SHELLCODE 0
# define CAVE_FOUND 1

extern uint64_t	load_size;
extern uint64_t	data_size;

int				g_flag;

typedef struct	s_woody
{
	uint64_t	text_addr;
	uint64_t	text_off;
	int64_t		addr_diff;
	uint64_t	key;
	Elf32_Addr	base_addr;
	uint64_t	inject_size;
	uint64_t	text_size;
	size_t		sh_strtbl_size;
	size_t		file_size;
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr	*shdr;
	uint64_t	new_entry;
	uint32_t	base_off;
	uint8_t		*load;
	char		*sh_stroff;
}				t_woody;

extern void		decrypt(void);

/*
** elf.c
*/

uint32_t		find_cave(char *ptr, size_t size, size_t inject);
uint32_t		get_base(t_woody *elf, char *ptr);
int				test_magic(Elf64_Ehdr *elf);
void			get_sh(char  *ptr, t_woody *elf);
void			get_off(t_woody *elf);

/*
** util.c
*/

int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strcmp(const char *s1, const char *s2);
void			*ft_memcpy(void *dst, const void *src, size_t n);
int				return_error(char *text);

/*
** loader.c
*/

void			fill_load(uint8_t *load, t_woody *elf);
int				insert_decrypt(t_woody *elf);
void			write_new_file(char *ptr, t_woody *elf);

/*
** encrypt.c
*/

uint64_t		get_random_key(void);
uint64_t		rotate_right(uint64_t key);
void			encrypt_section(char *ptr, t_woody *elf);

/*
** woody.c
*/

void			woody(char *ptr, struct stat *buf);

#endif

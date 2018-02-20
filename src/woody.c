#include "../include/woody.h"

uint32_t	find_cave(char *ptr, size_t size, size_t inject)
{
	uint32_t	i;
	size_t		cave_size;

	i = 0;
	cave_size = 0;
	while (i <= size)
	{
		cave_size++;
		if (i != size && *(ptr + i) != 0)
			cave_size = 0;
		if (i == size || cave_size == inject)
			break ;
		i++;
	}
	return (i - cave_size);
}

uint32_t	get_base(t_woody *elf, char *ptr)
{
	size_t		i;
	Elf64_Phdr	*phdr;

	i = -1;
	phdr = (Elf64_Phdr *)(ptr + elf->ehdr->e_phoff);
	while (++i < elf->ehdr->e_phnum && phdr->p_type != 1)
		phdr = (Elf64_Phdr *)((char *)phdr + sizeof(Elf64_Phdr));
	elf->base_off = phdr->p_offset;
	return (phdr->p_vaddr);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	int				result;
	unsigned char	*tmp1;
	unsigned char	*tmp2;

	i = 0;
	tmp1 = (unsigned char *)s1;
	tmp2 = (unsigned char *)s2;
	while (tmp1[i] != '\0' && tmp2[i] != '\0' && i < n)
	{
		result = tmp1[i] - tmp2[i];
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if ((unsigned char)*s1 != (unsigned char)*s2)
			break ;
		s1++;
		s2++;
	}
	return ((int)(*s1 - *s2));
}

int			test_magic(Elf64_Ehdr *elf)
{
	if (elf->e_ident[0] != ELFMAG0 || elf->e_ident[1] != ELFMAG1 ||
			elf->e_ident[2] != ELFMAG2 || elf->e_ident[3] != ELFMAG3
			|| elf->e_ident[4] != ELFCLASS64)
		return (0);
	return (1);
}

void		get_sh(char *ptr, t_woody *elf)
{
	Elf64_Shdr	*hdr;

	hdr = (Elf64_Shdr *)(ptr + elf->ehdr->e_shoff +
			(elf->ehdr->e_shentsize * elf->ehdr->e_shstrndx));
	elf->sh_strtbl_size = hdr->sh_size;
	elf->sh_stroff = ptr + hdr->sh_offset;
	elf->shdr = (Elf64_Shdr *)(ptr + elf->ehdr->e_shoff);
}

void		get_off(t_woody *elf)
{
	Elf64_Shdr	*shdr;
	size_t		sh_num;
	int			i;

	shdr = elf->shdr;
	sh_num = elf->ehdr->e_shnum;
	while (sh_num)
	{
		if (!ft_strcmp(elf->sh_stroff + shdr->sh_name, ".text"))
		{
			elf->text_addr = shdr->sh_addr;
			elf->text_off = shdr->sh_offset;
			elf->text_size = shdr->sh_size;
			break ;
		}
		shdr = (Elf64_Shdr *)(((char *)shdr) + sizeof(Elf64_Shdr));
		sh_num--;
	}
}

int			return_error(char *text)
{
	if (!text)
		perror("woody_woodpacker");
	else
		dprintf(2, "woody_woodpacker: %s\n", text);
	return (1);
}

void	woody(char *ptr, struct stat *buf)
{
	t_woody		*elf;

	elf = (t_woody *)malloc(sizeof(t_woody));
	elf->ehdr = (Elf64_Ehdr *)ptr;
	elf->file_size = (*buf).st_size;
	if (test_magic(elf->ehdr))
	{
		elf->base_addr = get_base(elf, ptr);
		get_sh(ptr, elf);
		get_off(elf);
		ecnrypt_section(ptr, elf);
		elf->new_entry = find_cave(ptr, elf->file_size, elf->inject_size);
		g_flag = insert_decrypt(ptr, elf);
		write_new_file(ptr, elf);
	}
	else
		return_error("not an elf64 file type");
	if (munmap(ptr, elf->file_size) == -1)
		return_error(NULL);
	free(elf->load);
	free(elf);
}

int			main(int argc, char **argv)
{
	int				fd;
	struct stat		buf;
	char			*ptr;

	if (argc != 2)
	{
		dprintf(2, "usage: [filename]\n");
		return (1);
	}
	if ((fd = open(argv[1], O_RDWR)) < 0)
		return (return_error(NULL));
	if (syscall(5, fd, &buf) < 0)
		return (return_error(NULL));
	if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
					fd, 0)) == MAP_FAILED)
		return (return_error(NULL));
	woody(ptr, &buf);
	return (0);
}

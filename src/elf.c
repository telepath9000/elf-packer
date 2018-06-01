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
	phdr->p_flags |= PF_W;
	printf("%u\n", phdr->p_flags);
	return (phdr->p_vaddr);
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

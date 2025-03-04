#include "../include/packer.h"

static int	set_phdr_flag(Elf64_Phdr *phdr_cur)
{
	if (phdr_cur->p_type == PT_LOAD) {
		phdr_cur->p_flags = PF_X | PF_W | PF_R;
		return 1;
	}
	return 0;
}

int			set_phdr_flags(Elf64_Ehdr *ehdr, char *file)
{
	Elf64_Off	end_point;
	int			phdr_exists;

	phdr_exists = 0;
	end_point = (Elf64_Off)((ehdr->e_phentsize * ehdr->e_phnum) + ehdr->e_phoff);
	for (Elf64_Off offset = ehdr->e_phoff; offset < end_point; offset += ehdr->e_phentsize)
		phdr_exists |= set_phdr_flag((Elf64_Phdr *)(file + offset));
	return phdr_exists;
}

static char	*get_strtable(Elf64_Ehdr *ehdr, char *file)
{
	uint64_t	shdr_off;

	shdr_off = ehdr->e_shoff + (ehdr->e_shstrndx * ehdr->e_shentsize);
	return file + (((Elf64_Shdr *)(file + shdr_off))->sh_offset);
}

static int	set_shdr_flag(Elf64_Shdr *shdr_cur, t_elf *bin, char *strtable)
{
	if (!strncmp(strtable + shdr_cur->sh_name, TO_ENCRYPT, strlen(TO_ENCRYPT))) {
		bin->encrypt_off = shdr_cur->sh_offset;
		bin->encrypt_addr = shdr_cur->sh_addr;
		bin->section_size = shdr_cur->sh_size;
		shdr_cur->sh_flags |= SHF_WRITE;
		return 1;
	}
	return 0;
}

int			set_shdr_flags(Elf64_Ehdr *ehdr, char *file, t_elf *bin)
{
	Elf64_Off	end_point;
	char		*strtable;

	end_point = (Elf64_Off)((ehdr->e_shentsize * ehdr->e_shnum) + ehdr->e_shoff);
	strtable = get_strtable(ehdr, file);
	for (Elf64_Off offset = ehdr->e_shoff; offset < end_point; offset += ehdr->e_shentsize)
		if (set_shdr_flag((Elf64_Shdr *)(file + offset), bin, strtable))
			return 1;
	return 0;
}

void		prepare_payload(t_elf *bin)
{
	bin->payload = malloc(load_size);
	memcpy(bin->payload, (void *)inject, load_size);
	memcpy(bin->payload + load_size - 24, &bin->enc_key, sizeof(uint64_t));
	memcpy(bin->payload + load_size - 16, &bin->encrypt_addr, sizeof(uint64_t));
	memcpy(bin->payload + load_size - 8, &bin->section_size, sizeof(uint64_t));
}

size_t		get_empty_space(size_t empty_space, char c)
{
	return c == 0 ? empty_space + 1 : 0;
}

uint64_t	find_fill_inject_point(t_elf *bin)
{
	size_t		empty_space;
	uint64_t	new_start;

	empty_space = 0;
	new_start = 0;
	for (size_t i = 0; i < bin->file_size; i++) {
		empty_space = get_empty_space(empty_space, bin->file_ptr[i]);
		if (empty_space == 0)
			new_start = i;
		if (empty_space == bin->payload_size) {
			printf("i: %li, file_size: %li", i, bin->file_size);
			memcpy(bin->file_ptr + new_start, bin->payload, bin->payload_size);
			return new_start;
		}
	}
	return 0;
}

int			prepare_file(t_elf *bin)
{
	if (!set_phdr_flags(bin->e_hdr, bin->file_ptr))
		return 0;
	if (!set_shdr_flags(bin->e_hdr, bin->file_ptr, bin))
		return 0;
	prepare_payload(bin);
	/*
	** there needs to be a guard for failure here, this can fail on memcpy if no
	** sufficiently large gap empty space is found.
	*/
	bin->new_entry = find_fill_inject_point(bin);
	bin->e_hdr->e_entry = bin->new_entry;
	return 1;
}

int			validate_elf64(Elf64_Ehdr *hdr)
{
	if (hdr->e_ident[EI_MAG0] == ELFMAG0 &&
			hdr->e_ident[EI_MAG1] == ELFMAG1 &&
			hdr->e_ident[EI_MAG2] == ELFMAG2 &&
			hdr->e_ident[EI_MAG3] == ELFMAG3 &&
			hdr->e_ident[EI_CLASS] == ELFCLASS64 &&
			(hdr->e_type == ET_EXEC || hdr->e_type == ET_DYN))
		return 1;
	return 0;
}

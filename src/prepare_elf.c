#include "../include/packer.h"

static int	phdr_flag_loop(Elf64_Off offset, Elf64_Off end_point, uint16_t p_block_size, char *file)
{
	Elf64_Phdr	*phdr_cur;
	int			phdr_exists;

	phdr_exists = 0;
	for (; offset < end_point; offset += p_block_size) {
		phdr_cur = (Elf64_Phdr *)(file + offset);
		if (phdr_cur->p_type == PT_LOAD) {
			phdr_cur->p_flags = PF_X | PF_W | PF_R;
			phdr_exists = 1;
		}
	}
	return phdr_exists;
}

int			set_phdr_flags(Elf64_Ehdr *ehdr, char *file)
{
	Elf64_Off	end_point;

	end_point = (Elf64_Off)((ehdr->e_phentsize * ehdr->e_phnum) + ehdr->e_phoff);
	return phdr_flag_loop(ehdr->e_phoff, end_point, ehdr->e_phentsize, file);
}

static char	*get_strtable(Elf64_Ehdr *ehdr, char *file)
{
	uint64_t	shdr_off;

	shdr_off = ehdr->e_shoff + (ehdr->e_shstrndx * ehdr->e_shentsize);
	return file + (((Elf64_Shdr *)(file + shdr_off))->sh_offset);
}

int			set_shdr_flags(Elf64_Ehdr *ehdr, char *file, t_elf *bin)
{
	Elf64_Shdr	*shdr_cur;
	Elf64_Off	off_cur;
	Elf64_Off	end_point;
	char		*strtable;
	char		*cur_name;
	int			ret;

	ret = 0;
	cur_name = NULL;
	off_cur = ehdr->e_shoff;
	end_point = (Elf64_Off)((ehdr->e_shentsize * ehdr->e_shnum) + off_cur);
	strtable = get_strtable(ehdr, file);
	for (; off_cur < end_point; off_cur += ehdr->e_shentsize) {
		shdr_cur = (Elf64_Shdr *)(file + off_cur);
		cur_name = strtable + shdr_cur->sh_name;
		if (!strncmp(cur_name, TO_ENCRYPT, strlen(TO_ENCRYPT))) {
			ret = 1;
			bin->encrypt_off = shdr_cur->sh_offset;
			bin->encrypt_addr = shdr_cur->sh_addr;
			bin->section_size = shdr_cur->sh_size;
			shdr_cur->sh_flags |= SHF_WRITE;
			break ;
		}
	}
	return ret;
}

void		prepare_payload(t_elf *bin)
{
	bin->payload = malloc(load_size);
	memcpy(bin->payload, (void *)inject, load_size);
	memcpy(bin->payload + load_size - 24, &bin->enc_key, sizeof(uint64_t));
	memcpy(bin->payload + load_size - 16, &bin->encrypt_addr, sizeof(uint64_t));
	memcpy(bin->payload + load_size - 8, &bin->section_size, sizeof(uint64_t));
}

uint64_t	find_fill_inject_point(t_elf *bin)
{
	size_t		empty_space;
	size_t		empty_start_tmp;
	long long	empty_start;
	size_t		i;
	uint64_t	ret;

	empty_space = 0;
	empty_start_tmp = 0;
	empty_start = -1;
	for (i = 0; i < bin->file_size; i++) {
		if (bin->file_ptr[i] == 0) {
			if (empty_space == 0)
				empty_start_tmp = i;
			empty_space++;
			if (empty_space == bin->payload_size) {
				empty_start = empty_start_tmp;
				break ;
			}
		}
		else {
			empty_space = 0;
		}
	}
	if (empty_start == -1) {
		bin->in_file = 1;
		ret = bin->file_size;
	}
	else {
		ret = empty_start;
		memcpy(bin->file_ptr + empty_start, bin->payload, bin->payload_size);
	}
	bin->e_hdr->e_entry = ret;
	return ret;
}

int			prepare_file(t_elf *bin)
{
	
	if (!set_phdr_flags(bin->e_hdr, bin->file_ptr))
		return 0;
	if (!set_shdr_flags(bin->e_hdr, bin->file_ptr, bin))
		return 0;
	prepare_payload(bin);
	bin->new_entry = find_fill_inject_point(bin);
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

#include "../include/packer.h"

int		print_error_return(int e_flag)
{
	switch(e_flag) {
		case e_bad_args:
			fprintf(stderr, "Incorrect number of args.\n");
			break;
		case e_stat_fail:
			fprintf(stderr, "Stat failed.\n");
			break;
		case e_regfile_fail:
			fprintf(stderr, "Argument not a file.\n");
			break;
		case e_open_fail:
			fprintf(stderr, "Couldn't open file provided as argument.\n");
			break;
		case e_mmap_fail:
			fprintf(stderr, "Couldn't map file to memory,\n");
			break;
		default:
			fprintf(stderr, "ERROR.\n");
	}
	return (1);
}

void	print_error(int e_flag)
{
	switch(e_flag) {
		case e_elf64_fail:
			fprintf(stderr, "File not a 64 bit ELF format executable.\n");
			break;
		case e_malloc_fail:
			fprintf(stderr, "Malloc failure.\n");
			break;
		case e_modify_fail:
			fprintf(stderr, "Could not modify source file for some reason.\n");
			break;
		case e_write_fail:
			fprintf(stderr, "Could not write to new file.\n");
			break;
		default:
			fprintf(stderr, "ERROR.\n");
	}
}

int		init_t_elf(char *file, struct stat *statbuf, t_elf *bin)
{
	srand(time(0));
	bin->e_hdr = (Elf64_Ehdr *)file;
	bin->p_hdr = (bin->e_hdr->e_phoff > 0) ?
		((Elf64_Phdr *)(file + bin->e_hdr->e_phoff)) : NULL;
	bin->s_hdr = (bin->e_hdr->e_shoff > 0) ?
		((Elf64_Shdr *)(file + bin->e_hdr->e_shoff)) : NULL;
	bin->new_entry = 0;
	bin->encrypt_addr = 0;
	bin->encrypt_off = 0;
	bin->enc_key = gen_key();
	bin->section_size = 0;
	bin->file_size = statbuf->st_size;
	bin->in_file = 0;
	bin->payload = NULL; 
	bin->payload_size = load_size;
	bin->file_ptr = file;
	return 1;
}

int		write_file(t_elf *bin)
{
	int		fd;
	int		flags;
	int		ret;

	ret = 1;
	if (bin->in_file == 1)
		flags = (O_RDWR | O_CREAT);
	else
		flags = (O_WRONLY | O_CREAT | O_APPEND);
	if ((fd = open(NEW_FILE, flags, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) == -1)
		ret = 0;
	else if (ret) {
		if (bin->in_file == 1 && write(fd, bin->file_ptr, bin->file_size) == -1)
			ret = 0;
		else if (write(fd, bin->file_ptr, bin->file_size) == -1 ||
				write(fd, bin->payload, bin->payload_size) == -1)
			ret = 0;
	}
	if (ret)
		close(fd);
	return (ret);
}

void	destruct(t_elf *bin)
{
	if (bin->payload)
        free(bin->payload);
}

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

t_elf	*init_t_elf(char *file, struct stat *statbuf)
{
	t_elf	*ret;

	srand(time(0));
	ret = malloc(sizeof(t_elf));
	ret->e_hdr = (void *)file;
	ret->p_hdr = (ret->e_hdr->e_phoff > 0) ?
		((void *)(file + ret->e_hdr->e_phoff)) : NULL;
	ret->s_hdr = (ret->e_hdr->e_shoff > 0) ?
		((void *)(file + ret->e_hdr->e_shoff)) : NULL;
	ret->new_entry = 0;
	ret->encrypt_addr = 0;
	ret->encrypt_off = 0;
	ret->enc_key = gen_key();
	ret->section_size = 0;
	ret->file_size = statbuf->st_size;
	ret->in_file = 0;
	ret->payload = NULL; 
	ret->payload_size = load_size;
	ret->file_ptr = file;
	return ret;
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
	if (bin) {
        free(bin->payload);
        free(bin);
    }
}

#include "../include/packer.h"

int		main(int argc, char **argv)
{
	t_elf		*bin;
	struct stat	statbuf;
	char		*file_buf;
	int			fd;

	bin = NULL;
	file_buf = NULL;
	if (argc != 2)
		return (print_error_return(e_bad_args));
	if (stat(argv[1], &statbuf) == -1)
		return (print_error_return(e_stat_fail));
	if (!(S_ISREG(statbuf.st_mode)))
		return (print_error_return(e_regfile_fail));
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		return (print_error_return(e_open_fail));
	if ((file_buf = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE,
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (print_error_return(e_mmap_fail));
	if (!validate_elf64((Elf64_Ehdr *)file_buf))
		print_error(e_elf64_fail);
	else if ((bin = init_t_elf(file_buf, statbuf)) == NULL)
		print_error(e_malloc_fail);
	else if (!prepare_file(bin))
		print_error(e_modify_fail);
	else if (!encrypt_section(bin))
		print_error(e_encrypt_fail);
	else if (!write_file(bin))
		print_error(e_write_fail);
	munmap(file_buf, statbuf.st_size);
	destruct(bin);
	return (0);
}

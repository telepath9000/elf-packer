#include "../include/woody.h"

void	woody(char *ptr, struct stat *buf)
{
	t_woody		*elf;

	elf = (t_woody *)malloc(sizeof(t_woody));
	elf->ehdr = (Elf64_Ehdr *)ptr;
	elf->file_size = (*buf).st_size;
	if (test_magic(elf->ehdr))
	{
		elf->base_addr = get_base(elf, ptr);
		printf("base_addr in woody: %lx\n", elf->base_addr);
		get_sh(ptr, elf);
		get_off(elf);
		encrypt_section(ptr, elf);
		g_flag = insert_decrypt(elf);
		elf->new_entry = find_cave(ptr, elf->file_size, load_size);
		printf("the  new entry point is: %u\n", elf->new_entry);
		printf("injecct size: %lu\n", load_size);
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

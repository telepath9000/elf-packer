#include "../include/woody.h"

int		woody(char *ptr)
{

}

int		return_error(void)
{
	perror("woody_woodpacker: ");
	return (1);
}

int		main(int argc, char **argv)
{
	int			fd;
	struct stat	buf
	char		*ptr;
	
	if (argc != 2)
	{
		write(2, "usage: [filename]\n", 18);
		return (1);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		return (return_error());
	if (fstat(fd, &buf) < 0)
		return (return_error());
	if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
					fd, 0)) == MAP_FAILED)
		return (return_error());
	woody(ptr);
	if (save_unmap(ptr) == -1)
		return (1);
	return (0);
}

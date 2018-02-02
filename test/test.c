#include <stdio.h>
#include <elf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	Elf64_Ehdr	ehdr;
	char		buf[100];
	int			fd;

	bzero(buf, 100);
	fd = open(argv[1], O_RDWR);
	lseek(fd, 6293784, SEEK_SET);
	read(fd, buf, sizeof(buf));
	write(1, buf, 100);
/*	read(fd, &ehdr, sizeof(ehdr));
	printf("Entry: %lu\n", ehdr.e_entry);
//	ehdr.e_entry += 10000000000;
	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(ehdr));*/
	close(fd);
	return (0);
}

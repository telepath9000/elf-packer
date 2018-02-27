#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

int main(void)
{
	int 		fd;
	struct stat buf;
	int64_t	dest;
	int			bytes_read;
	char		*ptr;

	fd = open("tmp", O_RDONLY);
	stat("tmp", &buf);
	ptr = (char *)calloc(buf.st_size + 1, sizeof(char));
	read(fd, ptr, buf.st_size);
	dest = *((int64_t *)ptr);
	printf("%li\n", dest);
	return (0);
}

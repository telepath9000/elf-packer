#include "../include/woody.h"

void	fill_load(uint8_t *load, t_woody *elf)
{
	ft_memcpy(load, (void *)decrypt, load_size);
	ft_memcpy(load + load_size - 32, &elf->key, sizeof(uint64_t));
	ft_memcpy(load + load_size - 24, &elf->text_addr, sizeof(uint64_t));
	ft_memcpy(load + load_size - 16, &elf->text_size, sizeof(uint64_t));
	ft_memcpy(load + load_size - 8, &elf->new_entry, sizeof(uint64_t));
}

int		insert_decrypt(t_woody *elf)
{
	elf->load = (uint8_t *)malloc(load_size);
	fill_load(elf->load, elf);
	ft_memcpy(elf->load + load_size - (data_size + 4), &elf->ehdr->e_entry, 4);
	elf->ehdr->e_entry = elf->base_addr + elf->new_entry;
	if (elf->new_entry >= elf->file_size)
		return (APPEND_SHELLCODE);
	else
		return (CAVE_FOUND);
}

void	write_new_file(char *ptr, t_woody *elf)
{
	int		fd;

	if ((fd = open("./woody", O_WRONLY | O_APPEND | O_CREAT,
					S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		return_error(NULL);
		return ;
	}
	if (g_flag == CAVE_FOUND)
	{
		ft_memcpy(ptr + elf->new_entry, elf->load, load_size);
		write(fd, ptr, elf->file_size);
	}
	else if (g_flag == APPEND_SHELLCODE)
	{
		write(fd, ptr, elf->file_size);
		write(fd, elf->load, load_size);
	}
	close(fd);
}

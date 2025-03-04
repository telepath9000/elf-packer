#ifndef PACKER_H
# define PACKER_H

# include <elf.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>
# include <time.h>

# define TO_ENCRYPT ".text"
# define NEW_FILE "./elf_encrypted"

extern uint64_t	load_size;

extern void		inject(void);

enum	error_flags {e_bad_args, e_stat_fail, e_regfile_fail, e_open_fail,
			e_mmap_fail, e_elf64_fail, e_malloc_fail, e_modify_fail,
			e_write_fail, e_encrypt_fail};

typedef struct	s_elf
{
	Elf64_Ehdr	*e_hdr;
	Elf64_Phdr	*p_hdr;
	Elf64_Shdr	*s_hdr;
	uint64_t	new_entry;
	uint64_t	encrypt_addr;
	uint64_t	encrypt_off;
	uint64_t	enc_key;
	size_t		section_size;
	size_t		file_size;
	size_t		payload_size;
	char		*payload;
	char		*file_ptr;
	int			in_file;
}				t_elf;

/* prepare_elf.c */

int			prepare_file(t_elf *bin);
int			validate_elf64(Elf64_Ehdr *hdr);
int			set_phdr_flags(Elf64_Ehdr *ehdr, char *file);
int			set_shdr_flags(Elf64_Ehdr *ehdr, char *file, t_elf *bin);
uint64_t	inject_payload(t_elf *bin);
void		prepare_payload(t_elf *bin);

/* util.c */

int			print_error_return(int e_flag); /* returns 1 */
void		print_error(int e_flag);
int			write_file(t_elf *bin);
int			init_t_elf(char *file, struct stat *statbuf, t_elf *bin);
void		destruct(t_elf *bin);

/* encrypt.c */

int			encrypt_section(t_elf *bin);
uint64_t	gen_key(void);

#endif /* PACKER_H */

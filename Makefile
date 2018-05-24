NAME = packer

SRC = src/woody.c src/loader.c src/util.c src/elf.c src/encrypt.c

SRC_ASM = src/decrypt.asm

OBJ_ASM = $(SRC_ASM:.asm=.o)

OBJ = $(SRC:.c=.o)

INC = ./include

CC = gcc

NASM = nasm

RM = rm -f

override CFLAGS += -Wall -Wextra -no-pie -g -I$(INC)

override NFLAGS += -f elf64

$(NAME): $(OBJ) $(OBJ_ASM)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(OBJ_ASM)

$(OBJ_ASM): $(SRC_ASM)
	$(NASM) $(NFLAGS) -o $@ $<

all: $(NAME)

clean:
	$(RM) $(OBJ) $(OBJ_ASM)

fclean: clean
	$(RM) $(NAME) woody

re: fclean all

.PHONY: all clean fclean re

NAME = packer

DEBUG = packer_debug

SRC = src/packer.c src/encrypt.c src/prepare_elf.c src/util.c

SRC_ASM = src/inject.asm

OBJ_ASM = $(SRC_ASM:.asm=.o)

OBJ = $(SRC:.c=.o)

INC = ./include

CC = gcc

NASM = nasm

RM = rm -f

override CFLAGS += -g -Wall -Wextra -I$(INC)

override NFLAGS += -f elf64

$(NAME): $(OBJ) $(OBJ_ASM)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(OBJ_ASM)

$(DEBUG): $(OBJ) $(OBJ_ASM)
	$(CC) $(CFLAGS) -o $(DEBUG) $(OBJ) $(OBJ_ASM)

$(OBJ_ASM): $(SRC_ASM)
	$(NASM) $(NFLAGS) -o $@ $<

all: $(NAME)

debug: $(DEBUG)

clean:
	$(RM) $(OBJ) $(OBJ_ASM)

fclean: clean
	$(RM) $(NAME) $(DEBUG) vgcore.*

re: fclean all

.PHONY: all clean fclean re

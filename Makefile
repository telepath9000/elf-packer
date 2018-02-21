NAME = woody_woodpacker

SRC = src/woody.c src/loader.c src/util.c src/elf.c src/encrypt.c

SRC_ASM =

OBJ = $(SRC:.c=.o)

OBJ_ASM = $(SRC_ASM:.s=.o)

INC = ./include

CC = gcc

NASM = nasm

RM = rm -f

override CFLAGS += -Wall -Wextra -Werror -I$(INC)

override NFLAGS += -f elf64

$(NAME): $(OBJ) $(OBJ_ASM)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(OBJ_ASM)

$(OBJ_ASM): $(SRC_ASM)
	$(NASM) $(NFLAGS) -o $@ $<

all: $(NAME)

clean:
	$(RM) $(OBJ) $(OBJ_ASM)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

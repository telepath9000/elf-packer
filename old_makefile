NAME = woody_woodpacker
CC = gcc
NASM = nasm
NASMFLAGS = -f elf64
INC = -I include
CFLAGS = -Wall -Werror -Wextra -g
SRCS = woody.c

SRC = $(addprefix src/, $(SRCS))
OBJ = $(SRC:src/%.c=src/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC) $< -o $@
	
src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<
	@printf "\x1b[0;36mcompiled %s\x1b[0;39m\n" $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re all clean fclean

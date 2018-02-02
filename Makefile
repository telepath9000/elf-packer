NAME = woody_woodpacker
CC = gcc
NASM = nasm
NASMFLAGS = -f elf64
INC = -I include
CFLAGS = -Wall -Werror -Wextra -g
SRCS = woody.c
ASMSRC = 

SRC = $(addprefix src/, $(SRCS))
ASM = $(addprefix asm_src/, $(ASMSRC))
AOBJ = $(ASM:asm_src/%.s=asm_src/%.o)
OBJ = $(SRC:src/%.c=src/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(AOBJ)
	@printf "compiled %s" $<

asm_src/%.o: asm_src/%.s
	$(NASM) $(NASMFLAGS) $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	$(RM) $(AOBJ) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re all clean fclean

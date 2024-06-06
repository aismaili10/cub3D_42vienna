NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

HEADER = cub3D.h

SRC =	cub3D.c \
		map_val.c \
		map_val_utils.c \
		cleanup.c

OBJDIR = obj/
OBJ =  $(SRC:%.c=$(OBJDIR)%.o)

# Libraries
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
PRINTF_LIB = $(LIBFT_DIR)/printf/libftprintf.a

# Include directory
INCLUDES = -I$(LIBFT_DIR)

$(OBJDIR)%.o : %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJ) $(LIBFT) $(PRINTF_LIB) $(INCLUDES)

# Rule for making the libft library
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	-if [ -d $(OBJDIR) ]; then rmdir $(OBJDIR); fi
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

debug: CFLAGS += -g
debug: re

.PHONY: all re fclean clean debug
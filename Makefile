NAME = ft_ls

LIBFT = lib/libft.a

CC = cc

CFLAGS =	-Wall -Wextra -Werror -g \
			-Iinclude

LFLAGS =	-Llib -lft

SRC_DIR = src/
OBJ_DIR = obj/
FILES =	main/main main/utils main/sort main/display \
		parsing/parsing parsing/options parsing/errors \
		exec/ls exec/utils

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))

all: $(NAME)

bonus: all

clean :
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -rf $(NAME)
	@make fclean -C lib

re: fclean all

$(NAME): $(OBJS) $(LIBFT)
	@echo -e "\e[32m✔ Compilating sources files...\e[37m"
	@$(CC) -o $@ $(OBJS) $(LFLAGS)
	@echo -e "\e[32m✔ Executable created.\e[37m"

$(LIBFT):
	@make -C lib

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re bonus

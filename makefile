NAME      = cub3d
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -fsanitize=address
MLX_PATH  = ./minilibx-linux/
MLX_FLAGS = -L$(MLX_PATH) -lmlx -lXext -lX11 -lm

SRCS = \
	get_line/get_next_line.c \
	get_line/get_next_line_utils.c \
	parsing/utils.c \
	parsing/parsing.c \
	parsing/check_map.c \
	parsing/check_walls.c \
	parsing/mini_libft.c \
	src/main.c \
	src/init.c \
	src/movment.c \
	src/init_utils.c\
	src/loading_game.c \
	src/handle_keys.c \
	src/ray_casting_algo.c \


OBJS = $(SRCS:.c=.o)

all: $(MLX_PATH)libmlx.a $(NAME)

$(MLX_PATH)libmlx.a:
	@echo "Building MLX library..."
	@make -C $(MLX_PATH) --no-print-directory

$(NAME): $(OBJS) $(MLX_PATH)libmlx.a
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)
	@make -C $(MLX_PATH) clean --no-print-directory

fclean: clean
	rm -f $(NAME)
	@make -C $(MLX_PATH) clean --no-print-directory

re: fclean all

.PHONY: all clean fclean re

# ── Project ───────────────────────────────────────────────────────────
NAME    = lo3ba
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -fsanitize=address

# ── MiniLibX (Linux) ─────────────────────────────────────────────────
MLX_DIR   = ./minilibx-linux
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# ── Sources ───────────────────────────────────────────────────────────
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
	src/init_utils.c \
	src/loading_game.c \
	src/handle_keys.c \
	src/ray_casting_algo.c \

OBJS = $(SRCS:.c=.o)

# ── Rules ─────────────────────────────────────────────────────────────
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(MLX_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

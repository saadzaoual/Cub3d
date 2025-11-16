NAME      = lo3ba
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -I/usr/local/include
MLX_FLAGS = -lmlx -lXext -lX11 -lm -L/usr/local/lib

SRCS = \
	get_line/get_next_line.c \
	get_line/get_next_line_utils.c \
	parsing/parsing.c \
	src/main.c \
	src/init.c \
	src/movment.c \
	src/init_utils.c\
	src/loading_game.c \
	src/handle_keys.c \
	src/ray_casting_algo.c \
	src/textures.c \


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

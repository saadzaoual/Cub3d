NAME      = Cub3d
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -I/usr/local/include
MLX_FLAGS = -lmlx -lXext -lX11 -lm -L/usr/local/lib

# Colors
RED       = \033[0;31m
GREEN     = \033[0;32m
YELLOW    = \033[0;33m
BLUE      = \033[0;34m
CYAN      = \033[0;36m
RESET     = \033[0m
BOLD      = \033[1m

SRCS = \
	get_line/get_next_line.c \
	get_line/get_next_line_utils.c \
	parsing/parsing_utils1.c \
	parsing/parsing_utils2.c \
	parsing/colors/parsing.colors.c \
	parsing/colors/parsing.colors2.c \
	parsing/maps/parsing_map1.c \
	parsing/maps/parsing_map2.c \
	parsing/maps/parsing_map3.c \
	parsing/maps/parsing_map4.c \
	parsing/player/parcing_player.c \
	parsing/textures/parsing_textures.c \
	parsing/textures/parsing_textures2.c \
	parsing/textures/parsing_textures3.c \
	src/main.c \
	src/main_utils/main_utils.c \
	src/main_utils/main_utils2.c \
	src/main_utils/main_utils3.c \
	src/main_utils/main_utils4.c \
	src/main_utils/main_utils5.c \
	src/init/init.c \
	src/init/init_utils.c \
	src/movment/movment.c \
	src/movment/movment2.c \
	src/movment/movment3.c \
	src/game_loading/loading_game.c \
	src/game_loading/loading_game2.c \
	src/game_loading/loading_game3.c \
	src/keys/handle_keys.c \
	src/keys/handle_keys2.c \
	src/ray_cast/ray_casting_algo1.c \
	src/ray_cast/ray_casting_algo2.c \
	src/ray_cast/ray_casting_algo3.c \
	src/textures/textures.c

OBJS = $(SRCS:.c=.o)
TOTAL = $(words $(SRCS))
COUNT = 0

all: header $(NAME) footer

header:
	@echo ""
	@echo "$(CYAN)$(BOLD)╔══════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)$(BOLD)║                    🎮  CUB3D PROJECT  🎮                     ║$(RESET)"
	@echo "$(CYAN)$(BOLD)╚══════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""

%.o: %.c
	@$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
	@if [ $$(($(COUNT) % 6)) -eq 1 ]; then \
	    printf "$(CYAN)    ╱▔▔▔╲  $(RESET)[$(YELLOW)%2d$(RESET)/$(YELLOW)%2d$(RESET)] $(GREEN)%-40s$(RESET)\r" $(COUNT) $(TOTAL) "$<"; \
	 elif [ $$(($(COUNT) % 6)) -eq 2 ]; then \
	    printf "$(CYAN)   ╱ ▁▁ ╲ $(RESET)[$(YELLOW)%2d$(RESET)/$(YELLOW)%2d$(RESET)] $(GREEN)%-40s$(RESET)\r" $(COUNT) $(TOTAL) "$<"; \
	 elif [ $$(($(COUNT) % 6)) -eq 3 ]; then \
	    printf "$(CYAN)  ╱▁▁▁▁▁╲$(RESET)[$(YELLOW)%2d$(RESET)/$(YELLOW)%2d$(RESET)] $(GREEN)%-40s$(RESET)\r" $(COUNT) $(TOTAL) "$<"; \
	 elif [ $$(($(COUNT) % 6)) -eq 4 ]; then \
	    printf "$(CYAN)  ▏█████▕$(RESET)[$(YELLOW)%2d$(RESET)/$(YELLOW)%2d$(RESET)] $(GREEN)%-40s$(RESET)\r" $(COUNT) $(TOTAL) "$<"; \
	 elif [ $$(($(COUNT) % 6)) -eq 5 ]; then \
	    printf "$(CYAN)  ▏█▓▓█▕ $(RESET)[$(YELLOW)%2d$(RESET)/$(YELLOW)%2d$(RESET)] $(GREEN)%-40s$(RESET)\r" $(COUNT) $(TOTAL) "$<"; \
	 else \
	    printf "$(CYAN)  ╲▁▁▁▁▁╱$(RESET)[$(YELLOW)%2d$(RESET)/$(YELLOW)%2d$(RESET)] $(GREEN)%-40s$(RESET)\r" $(COUNT) $(TOTAL) "$<"; \
	 fi
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo "\n"
	@echo "$(YELLOW)  ╱▔▔▔▔▔╲  $(RESET)Linking objects..."
	@echo "$(YELLOW)  ▏█████▕  $(RESET)"
	@echo "$(YELLOW)  ╲▁▁▁▁▁╱  $(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

footer:
	@echo ""
	@echo "$(GREEN)$(BOLD)╔══════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(GREEN)$(BOLD)║                   ✓ BUILD SUCCESSFUL ✓                       ║$(RESET)"
	@echo "$(GREEN)$(BOLD)║                                                              ║$(RESET)"
	@echo "$(GREEN)$(BOLD)║   $(RESET)Executable: $(YELLOW)$(NAME)$(GREEN)$(BOLD)                                          ║$(RESET)"
	@echo "$(GREEN)$(BOLD)║   $(RESET)Usage: $(CYAN)./$(NAME) <map.cub>$(GREEN)$(BOLD)                                   ║$(RESET)"
	@echo "$(GREEN)$(BOLD)╚══════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)✓ Clean complete!$(RESET)"

fclean: clean
	@echo "$(RED)🗑️  Removing executable...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ Full clean complete!$(RESET)"

re: fclean all

.PHONY: all clean fclean re header footer

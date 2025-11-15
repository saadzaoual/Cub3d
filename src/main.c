/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 13:18:11 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

int	ft_strlen1(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*skip_spaces(char *str)
{
	if (!str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

char	*extract_path(char *line)
{
	char	*path;
	char	*end;
	int		len;

	if (!line)
		return (NULL);
	
	while (*line && *line != ' ' && *line != '\t')
		line++;
	
	line = skip_spaces(line);
	if (!*line)
		return (NULL);
	
	end = line;
	while (*end && *end != '\n' && *end != '\r')
		end++;
	
	while (end > line && (*(end - 1) == ' ' || *(end - 1) == '\t'))
		end--;
	
	len = end - line;
	if (len <= 0)
		return (NULL);
	
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	
	int i = 0;
	while (i < len)
	{
		path[i] = line[i];
		i++;
	}
	path[i] = '\0';
	
	return (path);
}

int	validate_color_format(char *color_str)
{
	int	i, commas, num_count;
	int	r, g, b;

	if (!color_str)
		return (0);
	
	i = 0;
	commas = 0;
	num_count = 0;
	
	while (color_str[i])
	{
		if (color_str[i] == ',')
			commas++;
		else if (color_str[i] >= '0' && color_str[i] <= '9')
			num_count++;
		else if (color_str[i] != ' ' && color_str[i] != '\t')
			return (0);
		i++;
	}
	
	if (commas != 2 || num_count < 3)
		return (0);
	
	char *ptr = color_str;
	
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	
	r = 0;
	int has_digits = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
	{
		r = r * 10 + (*ptr++ - '0');
		has_digits = 1;
	}
	if (!has_digits || r > 255) return (0);
	
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	if (*ptr != ',') return (0);
	ptr++;
	
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	
	g = 0;
	has_digits = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
	{
		g = g * 10 + (*ptr++ - '0');
		has_digits = 1;
	}
	if (!has_digits || g > 255) return (0);
	
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	if (*ptr != ',') return (0);
	ptr++;
	
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	
	b = 0;
	has_digits = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
	{
		b = b * 10 + (*ptr++ - '0');
		has_digits = 1;
	}
	if (!has_digits || b > 255) return (0);
	
	return (1);
}

int	validate_parsing_completeness(t_map *map)
{
	int	errors = 0;

	if (!map->no_texture)
	{
		printf("Error: Missing NO (North) texture\n");
		errors++;
	}
	if (!map->so_texture)
	{
		printf("Error: Missing SO (South) texture\n");
		errors++;
	}
	if (!map->we_texture)
	{
		printf("Error: Missing WE (West) texture\n");
		errors++;
	}
	if (!map->ea_texture)
	{
		printf("Error: Missing EA (East) texture\n");
		errors++;
	}
	if (!map->floor_color)
	{
		printf("Error: Missing F (Floor) color\n");
		errors++;
	}
	else if (!validate_color_format(map->floor_color))
	{
		printf("Error: Invalid floor color format: %s\n", map->floor_color);
		errors++;
	}
	if (!map->ceiling_color)
	{
		printf("Error: Missing C (Ceiling) color\n");
		errors++;
	}
	else if (!validate_color_format(map->ceiling_color))
	{
		printf("Error: Invalid ceiling color format: %s\n", map->ceiling_color);
		errors++;
	}
	
	return (errors == 0);
}

int	parse_map_config(t_map *map)
{
	int		i;
	char	*line;

	if (!map || !map->map)
		return (0);
	
	i = 0;
	while (map->map[i])
	{
		line = skip_spaces(map->map[i]);
		
		if (!*line || *line == '\n')
		{
			i++;
			continue;
		}
		
		if (ft_strncmp(line, "NO ", 3) == 0)
		{
			if (map->no_texture)
			{
				printf("Warning: Duplicate NO texture found\n");
				free(map->no_texture);
			}
			map->no_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "SO ", 3) == 0)
		{
			if (map->so_texture)
			{
				printf("Warning: Duplicate SO texture found\n");
				free(map->so_texture);
			}
			map->so_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "WE ", 3) == 0)
		{
			if (map->we_texture)
			{
				printf("Warning: Duplicate WE texture found\n");
				free(map->we_texture);
			}
			map->we_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "EA ", 3) == 0)
		{
			if (map->ea_texture)
			{
				printf("Warning: Duplicate EA texture found\n");
				free(map->ea_texture);
			}
			map->ea_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "F ", 2) == 0)
		{
			if (map->floor_color)
			{
				printf("Warning: Duplicate F color found\n");
				free(map->floor_color);
			}
			map->floor_color = extract_path(line);
		}
		else if (ft_strncmp(line, "C ", 2) == 0)
		{
			if (map->ceiling_color)
			{
				printf("Warning: Duplicate C color found\n");
				free(map->ceiling_color);
			}
			map->ceiling_color = extract_path(line);
		}
		else if (*line == '1' || *line == '0')
		{
		
			break;
		}
		
		i++;
	}
	
	return (1);
}

static void cleanup_map(t_map *map)
{
    if (!map)
        return;

    if (map->no_texture)
        free(map->no_texture);
    if (map->so_texture)
        free(map->so_texture);
    if (map->we_texture)
        free(map->we_texture);
    if (map->ea_texture)
        free(map->ea_texture);
    if (map->floor_color)
        free(map->floor_color);
    if (map->ceiling_color)
        free(map->ceiling_color);

    if (map->map)
    {
        for (int i = 0; i < map->height && map->map[i]; i++)
        {
            free(map->map[i]);
        }
        free(map->map);
    }

    if (map->img && map->mlx)
        mlx_destroy_image(map->mlx, map->img);
    if (map->win && map->mlx)
        mlx_destroy_window(map->mlx, map->win);
    if (map->mlx)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
    }

    free(map);
}
int	check_and_open_file(char *filename, t_map *map)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen1(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	if (filename[0] == '.' && len == 4)
		return (0);
    map->map_fd = open(filename, O_RDONLY);
    if (map->map_fd < 0)
		return (0);
    return (1);
}

int	read_file(t_map *map, char *filename)
{
	char	*line;
	char	**file_content;
	int		line_count;
	int		i;

	if (!map || !filename || map->map_fd < 0)
		return (0);
	
	line_count = 0;
	while ((line = get_next_line(map->map_fd)))
	{
		line_count++;
		free(line);
	}
	close(map->map_fd);
	
	if (line_count == 0)
		return (0);
	
	map->map_fd = open(filename, O_RDONLY);
	if (map->map_fd < 0)
		return (0);
	
	file_content = malloc(sizeof(char *) * (line_count + 1));
	if (!file_content)
	{
		close(map->map_fd);
		return (0);
	}
	
	i = 0;
	while ((line = get_next_line(map->map_fd)))
	{
		file_content[i] = line;
		i++;
	}
	file_content[i] = NULL;

	close(map->map_fd);

	map->map = file_content;
	map->height = i;

	map->width = 0;
	for (int j = 0; j < i; j++)
	{
		int len = 0;
		while (file_content[j][len] && file_content[j][len] != '\n' && file_content[j][len] != '\r')
			len++;
		if (len > map->width)
			map->width = len;
	}

	return (1);
}

static char get_map_char(t_map *map, int x, int y)
{
	if (y < 0 || y >= map->height)
		return '\0';  
	if (!map->map[y])
		return '\0';  
	if (x < 0)
		return '\0';  
	
	int len = 0;
	while (map->map[y][len] && map->map[y][len] != '\n' && map->map[y][len] != '\r')
		len++;
	
	if (x >= len)
		return '\0';  
	
	return map->map[y][x];
}


static int check_map_closed(t_map *map, int start_x, int start_y)
{
	
	char **visited = malloc(sizeof(char *) * map->height);
	if (!visited)
		return 0;
	
	for (int i = 0; i < map->height; i++)
	{
		visited[i] = calloc(map->width + 1, 1);
		if (!visited[i])
		{
			for (int j = 0; j < i; j++)
				free(visited[j]);
			free(visited);
			return 0;
		}
	}
	
	typedef struct s_point {
		int x, y;
	} t_point;
	
	t_point *stack = malloc(sizeof(t_point) * (map->width * map->height));
	if (!stack)
	{
		for (int i = 0; i < map->height; i++)
			free(visited[i]);
		free(visited);
		return 0;
	}
	
	int stack_size = 0;
	stack[stack_size++] = (t_point){start_x, start_y};
	visited[start_y][start_x] = 1;
	
	int is_closed = 1;  
	
	while (stack_size > 0)
	{
		t_point p = stack[--stack_size];
		
		char c = get_map_char(map, p.x, p.y);
		if (c == '\0')
		{
			is_closed = 0;  
			break;
		}
		
		if (c == '1')
			continue;
		
		int dx[] = {0, 1, 0, -1};
		int dy[] = {-1, 0, 1, 0};
		
		for (int i = 0; i < 4; i++)
		{
			int nx = p.x + dx[i];
			int ny = p.y + dy[i];
			
			if (ny < 0 || ny >= map->height || nx < 0 || nx >= map->width)
			{
				
				char nc = get_map_char(map, nx, ny);
				if (nc == '\0')
				{
					is_closed = 0;  
					stack_size = 0;  
					break;
				}
				continue;
			}
			
			if (!visited[ny][nx])
			{
				visited[ny][nx] = 1;
				stack[stack_size++] = (t_point){nx, ny};
			}
		}
	}
	
	free(stack);
	for (int i = 0; i < map->height; i++)
		free(visited[i]);
	free(visited);
	
	return is_closed;
}

int check_map_borders(t_map *map)
{
	if (!map || !map->map || map->height == 0)
		return 0;

	for (int x = 0; map->map[0][x] && map->map[0][x] != '\n' && map->map[0][x] != '\r'; x++)
	{
		char c = map->map[0][x];
		if (c != '1' && c != ' ' && c != '\t')
		{
			printf("Error\nFirst row must only contain walls ('1') or spaces\n");
			return 0;
		}
	}
	
	for (int x = 0; map->map[map->height - 1][x] && map->map[map->height - 1][x] != '\n' && map->map[map->height - 1][x] != '\r'; x++)
	{
		char c = map->map[map->height - 1][x];
		if (c != '1' && c != ' ' && c != '\t')
		{
			printf("Error\nLast row must only contain walls ('1') or spaces\n");
			return 0;
		}
	}

	for (int y = 0; y < map->height; y++)
	{
		if (!map->map[y])
			continue;
			
		int len = 0;
		while (map->map[y][len] && map->map[y][len] != '\n' && map->map[y][len] != '\r')
			len++;
		
		if (len == 0)
			continue;

		int first_char = 0;
		while (first_char < len && (map->map[y][first_char] == ' ' || map->map[y][first_char] == '\t'))
			first_char++;
		
		int last_char = len - 1;
		while (last_char >= 0 && (map->map[y][last_char] == ' ' || map->map[y][last_char] == '\t'))
			last_char--;
		
		if (first_char < len && map->map[y][first_char] != '1')
		{
			printf("Error\nRow %d must start with a wall ('1'), found '%c'\n", y, map->map[y][first_char]);
			return 0;
		}
		
		if (last_char >= 0 && map->map[y][last_char] != '1')
		{
			printf("Error\nRow %d must end with a wall ('1'), found '%c'\n", y, map->map[y][last_char]);
			return 0;
		}
	}

	return 1;
}

int validate_and_set_player(t_map *map)
{
	if (!map || !map->map)
		return 0;

	int player_count = 0;
	int player_x = -1, player_y = -1;  
	
	for (int y = 0; y < map->height; y++)
	{
		int x = 0;
		while (map->map[y][x] && map->map[y][x] != '\n' && map->map[y][x] != '\r')
		{
			char c = map->map[y][x];
			if (c == '1' || c == '0')
			{
				
			}
			else if (c == ' ' || c == '\t')
			{
				
				map->map[y][x] = '0';
			}
			else if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
			{
				player_count++;
				if (player_count > 1)
				{
					printf("Error\nMap contains more than one player spawn\n");
					cleanup_map(map);
					exit(1);
				}
				
				player_x = x;
				player_y = y;
				
				map->player.player_x = x * TILE + TILE / 2;
				map->player.player_y = y * TILE + TILE / 2;

				if (c == 'N')
					map->player.angle = 270.0;
				else if (c == 'S')
					map->player.angle = 90.0;
				else if (c == 'W')
					map->player.angle = 180.0;
				else 
					map->player.angle = 0.0;

				map->map[y][x] = '0';
				map->player_set = 1;
			}
			else
			{
				printf("Error\nInvalid character '%c' in map at (%d,%d)\n", c, y, x);
				cleanup_map(map);
				exit(1);
			}
			x++;
		}
	}

	if (player_count == 0)
	{
		printf("Error\nNo player spawn (N/S/W/E) found in map\n");
		cleanup_map(map);
		exit(1);
	}

	if (!check_map_closed(map, player_x, player_y))
	{
		printf("Error\nMap is not closed - player can escape to the outside\n");
		cleanup_map(map);
		exit(1);
	}

	return 1;
}

int	main(int ac, char **av)
{
	t_map	*map;

	if (ac != 2)
	{
		printf("Error\nUsage: ./cub3d <map.cub>\n");
		return (1);
	}
	
	map = malloc(sizeof(t_map));
	if (!map)
	{
		printf("Error\nMemory allocation failed\n");
		return (1);
	}
	
	map->map = NULL;
	map->map_fd = -1;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->floor_color = NULL;
	map->ceiling_color = NULL;
	map->mlx = NULL;
	map->win = NULL;
	map->img = NULL;
	map->img_data = NULL;
	
	map->keys.w = 0;
	map->keys.a = 0;
	map->keys.s = 0;
	map->keys.d = 0;
	map->keys.left = 0;
	map->keys.right = 0;
	
	if (!check_and_open_file(av[1], map))
	{
		printf("Error\nInvalid file format. Please provide a .cub file\n");
		free(map);
		return (1);
	}
	
	if (!read_file(map, av[1]))
	{
		printf("Error\nFailed to read file\n");
		cleanup_map(map);
		return (1);
	}
	
	if (!parse_map_config(map))
	{
		printf("Error\nFailed to parse map configuration\n");
		cleanup_map(map);
		return (1);
	}
	
	if (!validate_parsing_completeness(map))
	{
		printf("Error\nMap configuration is incomplete or invalid\n");
		cleanup_map(map);
		return (1);
	}

	int start_idx = 0;
	while (start_idx < map->height)
	{
		char *s = skip_spaces(map->map[start_idx]);
		if (s && (*s == '1' || *s == '0'))
			break;
		start_idx++;
	}
	if (start_idx >= map->height)
	{
		printf("Error\nNo map grid found in file\n");
		cleanup_map(map);
		return (1);
	}

	int end_idx = start_idx;
	int last_map_line = start_idx;
	while (end_idx < map->height)
	{
		char *s = skip_spaces(map->map[end_idx]);

		if (s && (*s == '1' || *s == '0'))
			last_map_line = end_idx;
		else if (s && *s != '\0')
		{

			printf("Error\nInvalid content after map grid at line %d\n", end_idx);
			cleanup_map(map);
			return (1);
		}
		end_idx++;
	}

	int map_lines = last_map_line - start_idx + 1;
	char **grid = malloc(sizeof(char *) * (map_lines + 1));
	if (!grid)
	{
		printf("Error\nMemory allocation failed\n");
		cleanup_map(map);
		return (1);
	}
	for (int k = 0; k < map_lines; k++)
		grid[k] = map->map[start_idx + k];
	grid[map_lines] = NULL;

	for (int k = 0; k < start_idx; k++)
		free(map->map[k]);
	free(map->map);

	map->map = grid;
	map->height = map_lines;

	map->width = 0;
	for (int j = 0; j < map->height; j++)
	{
		int len = 0;
		while (map->map[j][len] && map->map[j][len] != '\n' && map->map[j][len] != '\r')
			len++;
		if (len > map->width)
			map->width = len;
	}

	if (!check_map_borders(map))
	{
		cleanup_map(map);
		return (1);
	}

	if (!validate_and_set_player(map))
	{
		cleanup_map(map);
		return (1);
	}

	load_game(map);
	cleanup_map(map);
	return (0);
}

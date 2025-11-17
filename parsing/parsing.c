/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/15 00:00:00 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

static int	is_valid_xpm_path(const char *path)
{
	int		len;
	int		fd;
	char	buf[1];

	if (!path)
		return (0);
	len = ft_strlen1((char *)path);
	if (len < 5 || ft_strncmp(path + len - 4, ".xpm", 4) != 0)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	if (read(fd, buf, 1) <= 0)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
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
	else if (!is_valid_xpm_path(map->no_texture))
	{
		printf("Error: NO texture invalid or not found: %s\n", map->no_texture);
		errors++;
	}
	if (!map->so_texture)
	{
		printf("Error: Missing SO (South) texture\n");
		errors++;
	}
	else if (!is_valid_xpm_path(map->so_texture))
	{
		printf("Error: SO texture invalid or not found: %s\n", map->so_texture);
		errors++;
	}
	if (!map->we_texture)
	{
		printf("Error: Missing WE (West) texture\n");
		errors++;
	}
	else if (!is_valid_xpm_path(map->we_texture))
	{
		printf("Error: WE texture invalid or not found: %s\n", map->we_texture);
		errors++;
	}
	if (!map->ea_texture)
	{
		printf("Error: Missing EA (East) texture\n");
		errors++;
	}
	else if (!is_valid_xpm_path(map->ea_texture))
	{
		printf("Error: EA texture invalid or not found: %s\n", map->ea_texture);
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
				printf("Error\nDuplicate NO texture found\n");
				return (0);
			}
			map->no_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "SO ", 3) == 0)
		{
			if (map->so_texture)
			{
				printf("Error\nDuplicate SO texture found\n");
				return (0);
			}
			map->so_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "WE ", 3) == 0)
		{
			if (map->we_texture)
			{
				printf("Error\nDuplicate WE texture found\n");
				return (0);
			}
			map->we_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "EA ", 3) == 0)
		{
			if (map->ea_texture)
			{
				printf("Error\nDuplicate EA texture found\n");
				return (0);
			}
			map->ea_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "F ", 2) == 0)
		{
			if (map->floor_color)
			{
				printf("Error\nDuplicate F color found\n");
				return (0);
			}
			map->floor_color = extract_path(line);
		}
		else if (ft_strncmp(line, "C ", 2) == 0)
		{
			if (map->ceiling_color)
			{
				printf("Error\nDuplicate C color found\n");
				return (0);
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

void cleanup_map(t_map *map)
{
	int i;

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
		i = 0;
		while (i < map->height && map->map[i])
		{
			free(map->map[i]);
			i++;
		}
		free(map->map);
	}
	if (map->mlx)
	{
		if (map->north_tex.img)
			mlx_destroy_image(map->mlx, map->north_tex.img);
		if (map->south_tex.img)
			mlx_destroy_image(map->mlx, map->south_tex.img);
		if (map->west_tex.img)
			mlx_destroy_image(map->mlx, map->west_tex.img);
		if (map->east_tex.img)
			mlx_destroy_image(map->mlx, map->east_tex.img);
		if (map->img)
			mlx_destroy_image(map->mlx, map->img);
		if (map->win)
			mlx_destroy_window(map->mlx, map->win);
		mlx_destroy_display(map->mlx);
		free(map->mlx);
	}
	free(map);
}

char get_map_char(t_map *map, int x, int y)
{
	int len;

	if (y < 0 || y >= map->height)
		return '\0';
	if (!map->map[y])
		return '\0';
	if (x < 0)
		return '\0';
	len = 0;
	while (map->map[y][len] && map->map[y][len] != '\n' && map->map[y][len] != '\r')
		len++;
	if (x >= len)
		return '\0';
	return map->map[y][x];
}

typedef struct s_point
{
	int x;
	int y;
} t_point;

static int flood_fill_loop(t_map *map, t_point *stack, char **visited, int *stack_size)
{
	t_point p;
	char c;
	int dx[] = {0, 1, 0, -1};
	int dy[] = {-1, 0, 1, 0};
	int i;
	int nx;
	int ny;
	char nc;

	while (*stack_size > 0)
	{
		p = stack[--(*stack_size)];
		c = get_map_char(map, p.x, p.y);
		if (c == '\0')
			return 0;
		if (c == '1')
			continue;
		i = 0;
		while (i < 4)
		{
			nx = p.x + dx[i];
			ny = p.y + dy[i];
			if (ny < 0 || ny >= map->height || nx < 0 || nx >= map->width)
			{
				nc = get_map_char(map, nx, ny);
				if (nc == '\0')
				{
					*stack_size = 0;
					return 0;
				}
				i++;
				continue;
			}
			if (!visited[ny][nx])
			{
				visited[ny][nx] = 1;
				stack[(*stack_size)++] = (t_point){nx, ny};
			}
			i++;
		}
	}
	return 1;
}

int check_map_closed(t_map *map, int start_x, int start_y)
{
	char **visited;
	t_point *stack;
	int stack_size;
	int result;
	int i;
	int j;

	visited = malloc(sizeof(char *) * map->height);
	if (!visited)
		return 0;
	i = 0;
	while (i < map->height)
	{
		visited[i] = calloc(map->width + 1, 1);
		if (!visited[i])
		{
			j = 0;
			while (j < i)
				free(visited[j++]);
			free(visited);
			return 0;
		}
		i++;
	}
	stack = malloc(sizeof(t_point) * (map->width * map->height));
	if (!stack)
	{
		i = 0;
		while (i < map->height)
			free(visited[i++]);
		free(visited);
		return 0;
	}
	stack_size = 0;
	stack[stack_size++] = (t_point){start_x, start_y};
	visited[start_y][start_x] = 1;
	result = flood_fill_loop(map, stack, visited, &stack_size);
	free(stack);
	i = 0;
	while (i < map->height)
		free(visited[i++]);
	free(visited);
	return result;
}
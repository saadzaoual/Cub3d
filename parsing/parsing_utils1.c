/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/17 21:44:59 by szaoual          ###   ########.fr       */
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

char	*copy_string(char *src, int len)
{
	char	*dest;
	int		i;

	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*trim_end_spaces(char *start, char *end)
{
	while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t'))
		end--;
	return (end);
}

char	*skip_to_comma(char *ptr)
{
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	if (*ptr != ',')
		return (NULL);
	return (ptr + 1);
}

int	validate_parsing_completeness(t_map *map)
{
	int	valid_textures;
	int	valid_colors;

	valid_textures = validate_all_textures(map);
	valid_colors = validate_all_colors(map);
	return (valid_textures && valid_colors);
}

int	parse_config_line(t_map *map, char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (parse_no_texture(map, line));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (parse_so_texture(map, line));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (parse_we_texture(map, line));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (parse_ea_texture(map, line));
	else if (ft_strncmp(line, "F ", 2) == 0)
		return (parse_floor_color(map, line));
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (parse_ceiling_color(map, line));
	else if (*line == '1' || *line == '0')
		return (-1);
	return (1);
}

void	add_neighbor_to_stack(t_point *stack, char **visited, int *stack_size,
		t_point neighbor)
{
	if (!visited[neighbor.y][neighbor.x])
	{
		visited[neighbor.y][neighbor.x] = 1;
		stack[(*stack_size)++] = neighbor;
	}
}

int	process_neighbors(t_map *map, t_point p, t_point *stack, char **visited,
		int *stack_size)
{
	int	dx[4];
	int	dy[4];
	int	i;
	int	nx;
	int	ny;

	init_directions(dx, dy);
	i = 0;
	while (i < 4)
	{
		nx = p.x + dx[i];
		ny = p.y + dy[i];
		if (check_boundary_escape(map, nx, ny))
		{
			*stack_size = 0;
			return (0);
		}
		if (ny >= 0 && ny < map->height && nx >= 0 && nx < map->width)
			add_neighbor_to_stack(stack, visited, stack_size,
				(t_point){nx, ny});
		i++;
	}
	return (1);
}

char	**allocate_visited(t_map *map)
{
	char	**visited;
	int		i;
	int		j;

	visited = malloc(sizeof(char *) * map->height);
	if (!visited)
		return (NULL);
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
			return (NULL);
		}
		i++;
	}
	return (visited);
}

void	free_visited(char **visited, t_map *map)
{
	int	i;

	if (!visited)
		return ;
	i = 0;
	while (i < map->height)
		free(visited[i++]);
	free(visited);
}

t_point	*allocate_stack(t_map *map, char **visited)
{
	t_point	*stack;

	stack = malloc(sizeof(t_point) * (map->width * map->height));
	if (!stack)
	{
		free_visited(visited, map);
		return (NULL);
	}
	return (stack);
}
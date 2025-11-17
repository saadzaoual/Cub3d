/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/15 00:00:00 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"
#include "../parsing/parsing.h"

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

	map->floor_rgb = parse_rgb_color(map->floor_color);
	map->ceiling_rgb = parse_rgb_color(map->ceiling_color);

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
	int found_empty_after_map = 0;
	
	while (end_idx < map->height)
	{
		char *s = skip_spaces(map->map[end_idx]);

		if (s && (*s == '1' || *s == '0'))
		{
			if (found_empty_after_map)
			{
				printf("Error\nDuplicate map found at line %d\n", end_idx);
				cleanup_map(map);
				return (1);
			}
			last_map_line = end_idx;
		}
		else if (s && (*s == '\0' || *s == '\n' || *s == '\r'))
		{
			if (last_map_line >= start_idx)
				found_empty_after_map = 1;
		}
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
	for (int k = last_map_line + 1; k < map->height; k++)
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

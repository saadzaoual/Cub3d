/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/17 21:27:49 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	find_last_nonspace(char *row, int len)
{
	int	last_char;

	last_char = len - 1;
	while (last_char >= 0 && (row[last_char] == ' '
			|| row[last_char] == '\t'))
		last_char--;
	return (last_char);
}
int	check_row_borders(t_map *map, int y, int first_char, int last_char)
{
	if (first_char < get_row_length(map->map[y])
		&& map->map[y][first_char] != '1')
	{
		printf("Error\nRow %d must start with a wall ('1'), found '%c'\n",
			y, map->map[y][first_char]);
		return (0);
	}
	if (last_char >= 0 && map->map[y][last_char] != '1')
	{
		printf("Error\nRow %d must end with a wall ('1'), found '%c'\n",
			y, map->map[y][last_char]);
		return (0);
	}
	return (1);
}

int	check_middle_rows(t_map *map)
{
	int	y;
	int	len;
	int	first_char;
	int	last_char;
	y = 0;
	while (y < map->height)
	{
		if (!map->map[y] || !*map->map[y])
		{
			y++;
			continue;
		}
		len = get_row_length(map->map[y]);
		if (len == 0)
		{
			y++;
			continue;
		}
		first_char = find_first_nonspace(map->map[y], len);
		last_char = find_last_nonspace(map->map[y], len);
		if (!check_row_borders(map, y, first_char, last_char))
			return (0);
		y++;
	}
	return (1);
}

int	check_map_borders(t_map *map)
{
	if (!map || !map->map || map->height == 0)
		return (0);
	if (!check_first_row(map))
		return (0);
	if (!check_last_row(map))
		return (0);
	if (!check_middle_rows(map))
		return (0);
	return (1);
}
void	process_map_char(t_map *map, int x, int y, int *player_count)
{
	char	c;

	c = map->map[y][x];
	if (c == '1' || c == '0')
		return ;
	else if (c == ' ' || c == '\t')
		map->map[y][x] = '0';
	else if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		handle_player_spawn(map, x, y, player_count);
	else
	{
		printf("Error\nInvalid character '%c' in map at (%d,%d)\n", c, y, x);
		cleanup_map(map);
		exit(1);
	}
}
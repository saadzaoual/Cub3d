/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/17 21:28:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	check_map_closed(t_map *map, int start_x, int start_y)
{
	char	**visited;
	t_point	*stack;
	int		stack_size;
	int		result;

	visited = allocate_visited(map);
	if (!visited)
		return (0);
	stack = allocate_stack(map, visited);
	if (!stack)
		return (0);
	stack_size = 0;
	stack[stack_size++] = (t_point){start_x, start_y};
	visited[start_y][start_x] = 1;
	result = flood_fill_loop(map, stack, visited, &stack_size);
	free(stack);
	free_visited(visited, map);
	return (result);
}

void	cleanup_map_array(t_map *map)
{
	int	i;

	if (!map->map)
		return ;
	i = 0;
	while (i < map->height && map->map[i])
	{
		free(map->map[i]);
		i++;
	}
	free(map->map);
}
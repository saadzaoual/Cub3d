/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.colors2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/17 21:21:34 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	parse_ceiling_color(t_map *map, char *line)
{
	if (map->ceiling_color)
	{
		printf("Error\nDuplicate C color found\n");
		return (0);
	}
	map->ceiling_color = extract_path(line);
	return (1);
}
int	parse_floor_color(t_map *map, char *line)
{
	if (map->floor_color)
	{
		printf("Error\nDuplicate F color found\n");
		return (0);
	}
	map->floor_color = extract_path(line);
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/17 21:31:04 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	cleanup_mlx(t_map *map)
{
	if (!map->mlx)
		return ;
	cleanup_mlx_images(map);
	if (map->win)
		mlx_destroy_window(map->mlx, map->win);
	mlx_destroy_display(map->mlx);
	free(map->mlx);
}

void	cleanup_map(t_map *map)
{
	if (!map)
		return ;
	cleanup_textures(map);
	cleanup_map_array(map);
	cleanup_mlx(map);
	free(map);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 13:10:58 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

void clear_player_area(t_map *map)
{
    int x = -1;
    int y;
    int tile_x;
    int tile_y;
    int color;
    
    while (x <= 1)
    {
        y = -1;
        while (y <= 1)
        {
            tile_x = (map->player.player_x + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE + x;
            tile_y = (map->player.player_y + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE + y;

            if (tile_x >= 0 && tile_x < map->width && tile_y >= 0 && tile_y < map->height)
            {
                if (map->map[tile_y][tile_x] == '1')
                    color = COLOR_WALL;
                else if (map->map[tile_y][tile_x] == '0')
                    color = COLOR_FREE;
                else
                {
                    y++;
                    continue;
                }

                draw_square(map, tile_x, tile_y, color);
            }
            y++;
        }
        x++;
    }
}

int handle_close(t_map *map)
{
    if (map->win)
        mlx_destroy_window(map->mlx, map->win);
    if (map->mlx)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
    }
    exit(0);
    return 0;
}

int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;

    // Simple approach: just check the center tile of the player
    tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE;
    tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE;

    // Check bounds
    if (tile_x < 0 || tile_x >= map->width ||
        tile_y < 0 || tile_y >= map->height)
        return 0;

    // Check if center tile is free (not a wall)
    if (map->map[tile_y][tile_x] == '1')
        return 0;

    return 1;
}
    
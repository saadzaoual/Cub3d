/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:28 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 13:21:41 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

void pixel_put_img(t_map *map, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;

    int index = y * map->img_size_line + x * (map->img_bpp / 8);
    *(unsigned int *)(map->img_data + index) = color;
}

void draw_square(t_map *map, int x, int y, int color)
{
    int dy = 0;

    while (dy < TILE)
    {
        int dx = 0;
        while (dx < TILE)
        {
            pixel_put_img(map, x * TILE + dx, y * TILE + dy, color);
            dx++;
        }
        dy++;
    }
}

static int init_mlx_connection(t_map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        printf("Error: Failed to initialize MLX\n");
        return 0;
    }

    map->win = mlx_new_window(map->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "CUB3D - THE_LO3BA");
    if (!map->win)
    {
        printf("Error: Failed to create window\n");
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        return 0;
    }
    
    return 1;
}

static int init_rendering(t_map *map)
{
    map->img = mlx_new_image(map->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!map->img)
    {
        printf("Error: Failed to create image buffer\n");
        return 0;
    }
    
    map->img_data = mlx_get_data_addr(map->img, &map->img_bpp, 
                                      &map->img_size_line, &map->img_endian);
    if (!map->img_data)
    {
        printf("Error: Failed to get image data\n");
        return 0;
    }
    
    return 1;
}

void setup_player(t_map *map)
{
    if (map->player_set)
        return;
    
    for (int y = 0; y < map->height; y++)
    {
        
        if (!map->map[y])
            continue;
        
        for (int x = 0; x < map->width; x++)
        {
            
            if (map->map[y][x] && map->map[y][x] != '\n' && 
                map->map[y][x] != '\r' && map->map[y][x] == '0')
            {
                map->player.player_x = x * TILE + (TILE / 2);
                map->player.player_y = y * TILE + (TILE / 2);
                map->player.angle = 0.0;
                return;
            }
        }
    }
    
    map->player.player_x = TILE / 2;
    map->player.player_y = TILE / 2;
    map->player.angle = 0.0;
}

static void initial_render(t_map *map)
{
    render_game_view(map);
}

void load_game(t_map *map)
{
    if (!map)
    {
        printf("Error: Invalid map structure\n");
        exit(1);
    }
    
    if (!init_mlx_connection(map))
        exit(1);
    
    if (!init_rendering(map))
        exit(1);
    
    setup_player(map);
    setup_events(map);
    initial_render(map);
    
    mlx_loop(map->mlx);
}

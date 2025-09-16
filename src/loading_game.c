/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:28 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 11:41:15 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

void pixel_put_img(t_map *map, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= map->screen_width || y >= map->screen_height)
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

// Initialize MLX connection and window
static int init_mlx_connection(t_map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        printf("Error: Failed to initialize MLX\n");
        return 0;
    }

    map->win = mlx_new_window(map->mlx, map->screen_width, map->screen_height, "CUB3D - THE_LO3BA");
    if (!map->win)
    {
        printf("Error: Failed to create window\n");
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        return 0;
    }
    
    return 1;
}

// Initialize image buffer for rendering
static int init_rendering(t_map *map)
{
    map->img = mlx_new_image(map->mlx, map->screen_width, map->screen_height);
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
    
    printf("Rendering initialized: %dx%d buffer\n", map->screen_width, map->screen_height);
    return 1;
}

// Find and set initial player position
void setup_player(t_map *map)
{
    printf("Setting up player...\n");
    
    // Find player spawn position (N, S, E, W)
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            char spawn = map->map[y][x];
            if (spawn == 'N' || spawn == 'S' || spawn == 'E' || spawn == 'W')
            {
                map->player.player_x = x * TILE + (TILE / 2);
                map->player.player_y = y * TILE + (TILE / 2);
                
                // Set initial angle based on spawn direction
                switch (spawn)
                {
                    case 'N': map->player.angle = 270.0; break; // North (up)
                    case 'S': map->player.angle = 90.0; break;  // South (down)
                    case 'E': map->player.angle = 0.0; break;   // East (right)
                    case 'W': map->player.angle = 180.0; break; // West (left)
                }
                
                // Replace spawn character with free space
                map->map[y][x] = '0';
                
                printf("Player spawned at: (%.1f, %.1f) facing %c (angle: %.1f°)\n", 
                       map->player.player_x, map->player.player_y, spawn, map->player.angle);
                return;
            }
        }
    }
    
    // This should not happen if edge case validation passed
    printf("Error: No player spawn found (this should not happen)\n");
    exit(1);
}

// Initial render
static void initial_render(t_map *map)
{
    printf("Performing initial render...\n");
    
    // Clear screen
    // clear_image(map);
    
    // For now, just show a test screen or use 2D view
    // You can switch to render_game_view(map) when 3D is ready
    render_game_view(map);
}

// Main game loading function
void load_game(t_map *map)
{
    if (!map)
    {
        printf("Error: Invalid map structure\n");
        exit(1);
    }
    
    printf("Starting game initialization...\n");
    
    // Step 1: Initialize MLX and create window
    if (!init_mlx_connection(map))
        exit(1);
    
    // Step 2: Initialize rendering system
    if (!init_rendering(map))
        exit(1);
    
    // Step 3: Setup player
    setup_player(map);
    
    // Step 4: Setup event handlers
    setup_events(map);
    
    // Step 5: Initial render
    initial_render(map);
    
    printf("Game initialization complete. Starting main loop...\n");
    printf("Controls: Arrow keys to rotate, WASD to move, ESC to exit\n");
    
    // Start the game loop
    mlx_loop(map->mlx);
}

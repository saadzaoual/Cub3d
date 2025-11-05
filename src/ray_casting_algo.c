/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting_algo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:47:23 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 13:20:03 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"


t_ray cast_single_ray_3d(t_map *game, double ray_angle)
{
    t_ray ray = {0};
    // hona kaman nahtajo struct ofc wtf man 
    double ray_x;
    double ray_y;
    double ray_angle_rad;
    double ray_dx;
    double ray_dy;
    int map_x;
    int map_y;
    double delta_dist_x;
    double delta_dist_y;
    double side_dist_x;
    double side_dist_y;
    int step_x;
    int step_y;
    int hit;
    int side;
    double raw_distance;
    double angle_diff;
    double corrected_distance;

    // Initialize variables
    ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE / 2.0;
    ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE / 2.0;

    ray_angle_rad = DEG_TO_RAD(ray_angle);
    ray_dx = cos(ray_angle_rad);
    ray_dy = sin(ray_angle_rad);

    map_x = (int)(ray_x / TILE);
    map_y = (int)(ray_y / TILE);

    delta_dist_x = fabs(1.0 / ray_dx);
    delta_dist_y = fabs(1.0 / ray_dy);

    if (ray_dx < 0)
    {
        step_x = -1;
        side_dist_x = (ray_x / TILE - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - ray_x / TILE) * delta_dist_x;
    }

    if (ray_dy < 0)
    {
        step_y = -1;
        side_dist_y = (ray_y / TILE - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - ray_y / TILE) * delta_dist_y;
    }

    hit = 0;
    side = 0;

    while (!hit && map_x >= 0 && map_x < game->width && map_y >= 0 && map_y < game->height)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; // Vertical wall
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; // Horizontal wall
        }

        if (map_y >= 0 && map_y < game->height && map_x >= 0 && map_x < game->width)
        {
            /* Safety check: ensure the string is long enough before accessing */
            if (game->map[map_y] && game->map[map_y][map_x] && 
                game->map[map_y][map_x] != '\n' && game->map[map_y][map_x] != '\r')
            {
                if (game->map[map_y][map_x] == '1')
                    hit = 1;
            }
            else
            {
                /* Treat out-of-bounds or short lines as walls to prevent crashes */
                hit = 1;
            }
        }
    }

    if (hit)
    {
        if (side == 0)
        {
            raw_distance = (map_x - ray_x / TILE + (1 - step_x) / 2.0) / ray_dx;
            ray.wall_x = map_x * TILE;
            ray.wall_y = ray_y + raw_distance * ray_dy * TILE;
        }
        else
        {
            raw_distance = (map_y - ray_y / TILE + (1 - step_y) / 2.0) / ray_dy;
            ray.wall_y = map_y * TILE;
            ray.wall_x = ray_x + raw_distance * ray_dx * TILE;
        }

        angle_diff = DEG_TO_RAD(ray_angle - game->player.angle);
        while (angle_diff > M_PI) angle_diff -= 2 * M_PI;
        while (angle_diff < -M_PI) angle_diff += 2 * M_PI;

        corrected_distance = raw_distance * cos(angle_diff);
        ray.distance = corrected_distance * TILE;
        ray.hit_side = side;
    }

    return ray;
}




// 3D rendering function
void render_3d_view(t_map *game)
{

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            if (y < SCREEN_HEIGHT / 2)
                pixel_put_img(game, x, y, 0x87CEEB); // Sky blue ceiling
            else
                pixel_put_img(game, x, y, 0x8B4513); // Brown floor
        }
    }
    
    double fov = 60.0;
    double angle_step = fov / SCREEN_WIDTH;
    double start_angle = game->player.angle - (fov / 2.0);
    
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        double current_angle = start_angle + (x * angle_step);
        
        while (current_angle < 0) current_angle += 360;
        while (current_angle >= 360) current_angle -= 360;
        
        t_ray ray = cast_single_ray_3d(game, current_angle);
        
        if (ray.distance > 0)
        {
            int wall_height = (int)((TILE * SCREEN_HEIGHT) / ray.distance);
            
            int draw_start = (SCREEN_HEIGHT - wall_height) / 2;
            int draw_end = draw_start + wall_height;

            if (draw_start < 0) draw_start = 0;
            if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;
            
            int color;
            if (ray.hit_side == 0) // Vertical wall
                color = 0x404040; // White
            else // Horizontal wall  
                color = 0x404040; // Light gray (darker for shading)
            
            // Draw the wall slice
            for (int y = draw_start; y <= draw_end; y++)
            {
                pixel_put_img(game, x, y, color);
            }
        }
    }
}

void render_game_view(t_map *game)
{
    render_3d_view(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

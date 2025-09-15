#include "header/the_lo3ba.h"

// Add these constants to your header file if not already there:
// #define SCREEN_WIDTH 800
// #define SCREEN_HEIGHT 600 
// #define WALL_HEIGHT_FACTOR 100

// Modified cast_single_ray to return ray data instead of drawing
t_ray cast_single_ray_3d(t_map *game, double ray_angle)
{
    t_ray ray = {0, 0, 0, 0}; // Initialize ray struct
    
    // Get exact coordinates for the origin of each ray 
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2; 
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
    
    // Setting ray direction
    double ray_dx = cos(DEG_TO_RAD(ray_angle));
    double ray_dy = sin(DEG_TO_RAD(ray_angle));
    
    // Convert player position to grid coordinates
    int map_x = (int)(ray_x / TILE);
    int map_y = (int)(ray_y / TILE);

    // Calculate delta distances
    double delta_dist_x = fabs(1.0 / ray_dx);
    double delta_dist_y = fabs(1.0 / ray_dy);
    
    double side_dist_x, side_dist_y;
    int step_x, step_y;
    
    // Calculate step and initial side_dist
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
    
    int hit = 0;
    int side; // Track which side was hit
    
    // Perform DDA
    while (!hit && map_x >= 0 && map_x < game->width && map_y >= 0 && map_y < game->height)
    {
        // Jump to next map square, either in x-direction, or in y-direction
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
        
        // Check if ray has hit a wall
        if (map_x >= 0 && map_x < game->width && map_y >= 0 && map_y < game->height)
        {
            if (game->map[map_y][map_x] == '1')
                hit = 1;
        }
        else
            break;
    }
    
    if (hit)
    {
        // Calculate wall hit position and distance
        if (side == 0) // Vertical wall
        {
            ray.distance = (map_x - ray_x / TILE + (1 - step_x) / 2) / ray_dx * TILE;
            ray.wall_x = map_x * TILE + (step_x < 0 ? TILE : 0);
            ray.wall_y = ray_y + ray.distance * ray_dy;
        }
        else // Horizontal wall
        {
            ray.distance = (map_y - ray_y / TILE + (1 - step_y) / 2) / ray_dy * TILE;
            ray.wall_y = map_y * TILE + (step_y < 0 ? TILE : 0);
            ray.wall_x = ray_x + ray.distance * ray_dx;
        }
        
        ray.hit_side = side;
        
        // Apply fish-eye correction
        ray.distance = fabs(ray.distance * cos(DEG_TO_RAD(ray_angle - game->player.angle)));
    }
    
    return ray;
}

// 3D rendering function
void render_3d_view(t_map *game)
{
    // Clear the screen first (draw ceiling and floor)
    for (int y = 0; y < game->screen_height; y++)
    {
        for (int x = 0; x < game->screen_width; x++)
        {
            if (y < game->screen_height / 2)
                pixel_put_img(game, x, y, 0x87CEEB); // Sky blue ceiling
            else
                pixel_put_img(game, x, y, 0x8B4513); // Brown floor
        }
    }
    
    double fov = 60.0;
    double angle_step = fov / game->screen_width;
    double start_angle = game->player.angle - (fov / 2.0);
    
    for (int x = 0; x < game->screen_width; x++)
    {
        double current_angle = start_angle + (x * angle_step);
        
        // Normalize angle
        while (current_angle < 0) current_angle += 360;
        while (current_angle >= 360) current_angle -= 360;
        
        t_ray ray = cast_single_ray_3d(game, current_angle);
        
        if (ray.distance > 0)
        {
            // Calculate wall height based on distance
            int wall_height = (int)(WALL_HEIGHT_FACTOR * TILE / ray.distance);
            
            // Calculate drawing bounds
            int draw_start = (game->screen_height - wall_height) / 2;
            int draw_end = draw_start + wall_height;
            
            // Clamp to screen bounds
            if (draw_start < 0) draw_start = 0;
            if (draw_end >= game->screen_height) draw_end = game->screen_height - 1;
            
            // Choose wall color based on which side was hit
            int color;
            if (ray.hit_side == 0) // Vertical wall
                color = 0xFFFFFF; // White
            else // Horizontal wall  
                color = 0xCCCCCC; // Light gray (darker for shading)
            
            // Draw the wall slice
            for (int y = draw_start; y <= draw_end; y++)
            {
                pixel_put_img(game, x, y, color);
            }
        }
    }
}

// Wrapper function to replace your cast_fov_rays
void render_game_view(t_map *game)
{
    render_3d_view(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
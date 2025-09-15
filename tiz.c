#include "../header/the_lo3ba.h"

/* Game settings */
# define TILE 64
# define M_PI 3.14159265358979323846
# define DEG_TO_RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

/* Colors */
# define PLAYER_COLOR 0x00FF00    /* Green */
# define COLOR_WALL   0x4B0082    /* Indigo */
# define COLOR_FREE   0x006600    /* Dark Green */

# define MOVE_SPEED    8   // Pixels per keypress
# define PLAYER_SIZE   8   // Size of player square
# define PLAYER_OFFSET 12  // Offset from tile corner


#define SCREEN_WIDTH 520   // needs to be dynamiq based on map->height ant map->width
#define SCREEN_HEIGHT 400 // needss to be configirate to dynamiq
#define WALL_HEIGHT_FACTOR 100

/* Key codes */
# define KEY_ESC 65307
# define KEY_W   119
# define KEY_S   115
# define KEY_A   97
# define KEY_D   100

# define RAY_NUM 106 

typedef struct s_ray
{
    double wall_x;      // Wall hit x coordinate
    double wall_y;      // Wall hit y coordinate
    double distance;    // Distance to wall
    int hit_side;       // 0 if horizontal wall, 1 if vertical wall
}   t_ray;

typedef struct s_player
{
    double player_x;    // Player position coordinates
    double player_y;
    double angle;       // Player view direction in degrees
}   t_player;

typedef struct s_map
{
    void    *mlx;
    void    *win;
    void    *img;           // Image buffer
    char    *img_data;      // Image data pointer
    int     img_bpp;
    int     img_size_line;
    int     img_endian;
    char    **map;
    int     width;
    int     height;
    t_ray   rays[RAY_NUM];
    t_player player;
}   t_map;


// Modified cast_single_ray to return ray data instead of drawing
t_ray cast_single_ray_3d(t_map *game, double ray_angle)
{
    t_ray ray = {0, 0, 0, 0};
    
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2; 
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
    
    double ray_dx = cos(DEG_TO_RAD(ray_angle));
    double ray_dy = sin(DEG_TO_RAD(ray_angle));
    
    int map_x = (int)(ray_x / TILE);
    int map_y = (int)(ray_y / TILE);

    double delta_dist_x = fabs(1.0 / ray_dx);
    double delta_dist_y = fabs(1.0 / ray_dy);
    
    double side_dist_x, side_dist_y;
    int step_x, step_y;
    
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
    int side;
    double perpendicular_distance; // ADDED: For fish-eye correction
    
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
        // FIXED: Calculate perpendicular distance to prevent fish-eye
        if (side == 0) // Vertical wall
        {
            perpendicular_distance = (map_x - ray_x / TILE + (1 - step_x) / 2) / ray_dx;
            ray.wall_x = map_x * TILE + (step_x < 0 ? TILE : 0);
            ray.wall_y = ray_y + perpendicular_distance * ray_dy * TILE;
        }
        else // Horizontal wall
        {
            perpendicular_distance = (map_y - ray_y / TILE + (1 - step_y) / 2) / ray_dy;
            ray.wall_y = map_y * TILE + (step_y < 0 ? TILE : 0);
            ray.wall_x = ray_x + perpendicular_distance * ray_dx * TILE;
        }
        
        // CRITICAL: Apply fish-eye correction
        ray.distance = fabs(perpendicular_distance * TILE * cos(DEG_TO_RAD(ray_angle - game->player.angle)));
        ray.hit_side = side;
    }
    
    return ray;
}


// 3D rendering function
void render_3d_view(t_map *game)
{
    // Clear the screen first (draw ceiling and floor)
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
        
        // Normalize angle
        while (current_angle < 0) current_angle += 360;
        while (current_angle >= 360) current_angle -= 360;
        
        t_ray ray = cast_single_ray_3d(game, current_angle);
        
        if (ray.distance > 0)
        {
            // Calculate wall height based on distance
            int wall_height = (int)(WALL_HEIGHT_FACTOR * TILE / ray.distance);
            
            // Calculate drawing bounds
            int draw_start = (SCREEN_HEIGHT - wall_height) / 2;
            int draw_end = draw_start + wall_height;

            // Clamp to screen bounds
            if (draw_start < 0) draw_start = 0;
            if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;
            
            // Choose wall color based on which side was hit
            int color;
            if (ray.hit_side == 0) // Vertical wall
                color = 0xFF4B0082; // White
            else // Horizontal wall  
                color = 0xFF4B0082; // Light gray (darker for shading)
            
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

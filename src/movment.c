#include "../header/the_lo3ba.h"


int update_player(t_map *map)
{
    double new_x = map->player.player_x;
    double new_y = map->player.player_y;
    double angle = map->player.angle;

    // Handle rotation with smaller increments
    if (map->keys.left)
    {
        angle -= ROTATION_SPEED;
        if (angle < 0)
            angle += 360;
    }
    if (map->keys.right)
    {
        angle += ROTATION_SPEED;
        if (angle >= 360)
            angle -= 360;
    }

    // Movement vector accumulators
    double move_x = 0;
    double move_y = 0;

    if (map->keys.w)
    {
        move_x += cos(DEG_TO_RAD(angle)) * MOVE_SPEED;
        move_y += sin(DEG_TO_RAD(angle)) * MOVE_SPEED;
    }
    if (map->keys.s)
    {
        move_x -= cos(DEG_TO_RAD(angle)) * MOVE_SPEED;
        move_y -= sin(DEG_TO_RAD(angle)) * MOVE_SPEED;
    }
    if (map->keys.a)
    {
        move_x += cos(DEG_TO_RAD(angle - 90)) * MOVE_SPEED;
        move_y += sin(DEG_TO_RAD(angle - 90)) * MOVE_SPEED;
    }
    if (map->keys.d)
    {
        move_x += cos(DEG_TO_RAD(angle + 90)) * MOVE_SPEED;
        move_y += sin(DEG_TO_RAD(angle + 90)) * MOVE_SPEED;
    }

    new_x += move_x;
    new_y += move_y;

    // Check if move is valid
    if (is_valid_move(map, (int)new_x, (int)new_y))
    {
        map->player.player_x = new_x;
        map->player.player_y = new_y;
    }

    map->player.angle = angle;

    render_game_view(map);

    return 0;
}


int handle_key_release(int keycode, t_map *map)
{
    if (keycode == KEY_W)
        map->keys.w = 0;
    else if (keycode == KEY_A)
        map->keys.a = 0;
    else if (keycode == KEY_S)
        map->keys.s = 0;
    else if (keycode == KEY_D)
        map->keys.d = 0;
    else if (keycode == 65361) // left arrow
        map->keys.left = 0;
    else if (keycode == 65363) // right arrow
        map->keys.right = 0;

    return 0;
}

int handle_key_press(int keycode, t_map *map)
{
    if (keycode == KEY_ESC)
        return handle_close(map);

    if (keycode == KEY_W)
        map->keys.w = 1;
    else if (keycode == KEY_A)
        map->keys.a = 1;
    else if (keycode == KEY_S)
        map->keys.s = 1;
    else if (keycode == KEY_D)
        map->keys.d = 1;
    else if (keycode == 65361) // left arrow
        map->keys.left = 1;
    else if (keycode == 65363) // right arrow
        map->keys.right = 1;
    return 0;
}
void setup_events(t_map *map)
{
    printf("Setting up event handlers...\n");

    // Key press events
    mlx_hook(map->win, 2, 1L << 0, handle_key_press, map);

    // Key release events
    mlx_hook(map->win, 3, 1L << 1, handle_key_release, map);

    // Window close event
    mlx_hook(map->win, 17, 1L << 17, handle_close, map);

    // Setup continuous update loop for smooth movement + rotation
    mlx_loop_hook(map->mlx, update_player, map);
}
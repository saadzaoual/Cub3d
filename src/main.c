/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/01 23:04:50 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

static void validate_arguments(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Error: Invalid number of arguments\n");
        printf("Usage: %s <map_file.cub>\n", av[0]);
        exit(1);
    }
    // Note: check_ext() will be called in check_map(), so no need to duplicate here
}

static void cleanup_map(t_map *map)
{
    if (!map)
        return;

    // Free map array
    if (map->map)
    {
        for (int i = 0; i < map->height && map->map[i]; i++)
        {
            free(map->map[i]);
        }
        free(map->map);
    }

    // Cleanup MLX resources
    if (map->img && map->mlx)
        mlx_destroy_image(map->mlx, map->img);
    if (map->win && map->mlx)
        mlx_destroy_window(map->mlx, map->win);
    if (map->mlx)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
    }

    free(map);
}

int main(int ac, char **av)
{
    t_map *map;

    // Validate command line arguments
    validate_arguments(ac, av);

    printf("Loading and validating map: %s\n", av[1]);

    // Load, validate and check map (calls fill_map internally)
    map = check_map(av[1]);
    if (!map)
    {
        printf("Error: Failed to load or validate map\n");
        exit(1);
    }

    printf("Map validation successful: %dx%d\n", map->width, map->height);

    // Start the game
    load_game(map);

    // Cleanup resources (not reached if mlx_loop is running)
    cleanup_map(map);

    return 0;
}
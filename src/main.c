/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 13:18:11 by abnemili         ###   ########.fr       */
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

    validate_arguments(ac, av);
    printf("Loading lmap o dakchi ta tma %s\n", av[1]);
    map = check_map(av[1]);
    if (!map)
    {
        printf("Error: Failed to load eewa hadchi li 3lta lah\n");
        exit(1);
    }
    printf("lmap tvalidat tiiiir bt** : %dx%d\n", map->width, map->height);
    load_game(map);
    cleanup_map(map);

    return 0;
}
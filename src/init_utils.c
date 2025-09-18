/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:51:09 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 10:51:52 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

// Validate the map structure
static void validate_map(t_map *map)
{
    if (!map || !map->map || map->height <= 0)
    {
        puts("Error\nInvalid map structure\n");
        exit(1);
    }

    if (!map->map[0] || ft_strlen1(map->map[0]) <= 0)
    {
        puts("Error\nInvalid map content\n");
        exit(1);
    }

    printf("Map loaded: %dx%d\n", map->width, map->height);
}

// Filling the map from the file
t_map *fill_map(char *av)
{
    t_map *map;
    int h;

    h = map_height(av);
    if (h < 1)
    {
        puts("Error\nThe map is invalid\n");
        exit(1);
    }

    map = init_map_height(h);
    fill_map_content(map, av);

    if (!map->map[0])
    {
        puts("Error\nEmpty map file\n");
        exit(1);
    }

    map->width = ft_strlen1(map->map[0]);

    // Validate map before returning
    validate_map(map);

    return map;
}

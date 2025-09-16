/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edge_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: GitHub Copilot <copilot@github.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by GitHub Copilot   #+#    #+#             */
/*   Updated: 2025/09/15 00:00:00 by GitHub Copilot   ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

// Check for empty or null map
static void check_empty_map(t_map *map)
{
    if (!map || !map->map)
    {
        puts("Error\nEmpty or null map structure\n");
        exit(1);
    }
    
    if (map->height <= 0 || map->width <= 0)
    {
        puts("Error\nInvalid map dimensions\n");
        exit(1);
    }
}

// Check for map size limits (too small or too large)
static void check_map_size_limits(t_map *map)
{
    // Check minimum size - must be at least 3x3 to have walls and interior
    if (map->height < 3 || map->width < 3)
    {
        puts("Error\nMap too small (minimum 3x3)\n");
        exit(1);
    }
    
    // Check maximum size for performance and memory limits
    if (map->width > 200 || map->height > 200)
    {
        puts("Error\nMap too large (maximum 200x200)\n");
        exit(1);
    }
    
    // Check for reasonable screen dimensions after scaling
    int calculated_screen_width = map->width * TILE;
    int calculated_screen_height = map->height * TILE;
    
    if (calculated_screen_width > 3840 || calculated_screen_height > 2160)
    {
        puts("Error\nCalculated screen size too large (max 4K resolution)\n");
        exit(1);
    }
}

// Check for invalid characters in map
static void check_invalid_characters(t_map *map)
{
    int i, j;
    int player_count = 0;
    
    for (i = 0; i < map->height; i++)
    {
        for (j = 0; j < map->width; j++)
        {
            char c = map->map[i][j];
            
            // Valid characters: '0' (free space), '1' (wall), 'N', 'S', 'E', 'W' (player spawn)
            if (c != '0' && c != '1' && c != 'N' && c != 'S' && c != 'E' && c != 'W' && c != '\n')
            {
                printf("Error\nInvalid character '%c' at position [%d][%d]\n", c, i, j);
                exit(1);
            }
            
            // Count player spawn positions
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                player_count++;
            }
        }
    }
    
    // Check for exactly one player spawn
    if (player_count == 0)
    {
        puts("Error\nNo player spawn position found (N, S, E, or W)\n");
        exit(1);
    }
    else if (player_count > 1)
    {
        puts("Error\nMultiple player spawn positions found (only one allowed)\n");
        exit(1);
    }
}

// Check for inconsistent line lengths (non-rectangular map)
static void check_line_consistency(t_map *map)
{
    int i;
    int expected_width = map->width;
    
    for (i = 0; i < map->height; i++)
    {
        int line_length = ft_strlen1(map->map[i]);
        
        if (line_length != expected_width)
        {
            printf("Error\nInconsistent line length at row %d: expected %d, got %d\n", 
                   i, expected_width, line_length);
            exit(1);
        }
    }
}

// Check for holes in walls (boundary validation)
static void check_wall_integrity(t_map *map)
{
    int i, j;
    
    // Check all boundary walls
    for (i = 0; i < map->height; i++)
    {
        for (j = 0; j < map->width; j++)
        {
            // Skip if this is a wall
            if (map->map[i][j] == '1')
                continue;
            
            // If we're at the boundary, it must be a wall
            if (i == 0 || i == map->height - 1 || j == 0 || j == map->width - 1)
            {
                if (map->map[i][j] != '1')
                {
                    printf("Error\nNon-wall character at boundary [%d][%d]\n", i, j);
                    exit(1);
                }
            }
        }
    }
}

// Check for unreachable areas (flood fill validation)
static void flood_fill_check(t_map *map, int x, int y, char **visited)
{
    // Bounds check
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
        return;
    
    // Already visited or is a wall
    if (visited[y][x] == '1' || map->map[y][x] == '1')
        return;
    
    // Mark as visited
    visited[y][x] = '1';
    
    // Recursively check adjacent cells
    flood_fill_check(map, x + 1, y, visited);
    flood_fill_check(map, x - 1, y, visited);
    flood_fill_check(map, x, y + 1, visited);
    flood_fill_check(map, x, y - 1, visited);
}

// Check that all free spaces are reachable from player spawn
static void check_reachability(t_map *map)
{
    int i, j;
    int player_x = -1, player_y = -1;
    
    // Find player spawn position
    for (i = 0; i < map->height && player_x == -1; i++)
    {
        for (j = 0; j < map->width && player_x == -1; j++)
        {
            char c = map->map[i][j];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                player_x = j;
                player_y = i;
            }
        }
    }
    
    // Create visited array
    char **visited = malloc(sizeof(char *) * map->height);
    if (!visited)
        exit(1);
    
    for (i = 0; i < map->height; i++)
    {
        visited[i] = malloc(sizeof(char) * map->width);
        if (!visited[i])
            exit(1);
        
        for (j = 0; j < map->width; j++)
        {
            visited[i][j] = '0';
        }
    }
    
    // Perform flood fill from player position
    flood_fill_check(map, player_x, player_y, visited);
    
    // Check if all free spaces are reachable
    for (i = 0; i < map->height; i++)
    {
        for (j = 0; j < map->width; j++)
        {
            char map_char = map->map[i][j];
            if ((map_char == '0' || map_char == 'N' || map_char == 'S' || 
                 map_char == 'E' || map_char == 'W') && visited[i][j] == '0')
            {
                printf("Error\nUnreachable area at [%d][%d]\n", i, j);
                
                // Cleanup
                for (int k = 0; k < map->height; k++)
                    free(visited[k]);
                free(visited);
                exit(1);
            }
        }
    }
    
    // Cleanup
    for (i = 0; i < map->height; i++)
        free(visited[i]);
    free(visited);
}

// Main edge case checking function
void check_map_edge_cases(t_map *map)
{
    printf("Checking map edge cases...\n");
    
    // Basic structure validation
    check_empty_map(map);
    check_map_size_limits(map);
    
    // Content validation
    check_invalid_characters(map);
    check_line_consistency(map);
    
    // Wall and boundary validation
    check_wall_integrity(map);
    
    // Gameplay validation
    check_reachability(map);
    
    printf("Map edge case validation passed!\n");
}

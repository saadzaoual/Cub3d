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

int	ft_strlen1(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*skip_spaces(char *str)
{
	if (!str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

char	*extract_path(char *line)
{
	char	*path;
	char	*end;
	int		len;

	if (!line)
		return (NULL);
	
	// Skip the identifier (NO, SO, WE, EA)
	while (*line && *line != ' ' && *line != '\t')
		line++;
	
	// Skip spaces
	line = skip_spaces(line);
	if (!*line)
		return (NULL);
	
	// Find end of path (before newline or end of string)
	end = line;
	while (*end && *end != '\n' && *end != '\r')
		end++;
	
	// Remove trailing spaces
	while (end > line && (*(end - 1) == ' ' || *(end - 1) == '\t'))
		end--;
	
	len = end - line;
	if (len <= 0)
		return (NULL);
	
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	
	int i = 0;
	while (i < len)
	{
		path[i] = line[i];
		i++;
	}
	path[i] = '\0';
	
	return (path);
}

int	validate_color_format(char *color_str)
{
	int	i, commas, num_count;
	int	r, g, b;

	if (!color_str)
		return (0);
	
	i = 0;
	commas = 0;
	num_count = 0;
	
	// Count commas and check format
	while (color_str[i])
	{
		if (color_str[i] == ',')
			commas++;
		else if (color_str[i] >= '0' && color_str[i] <= '9')
			num_count++;
		else if (color_str[i] != ' ' && color_str[i] != '\t')
			return (0); // Invalid character
		i++;
	}
	
	if (commas != 2 || num_count < 3)
		return (0);
	
	// Parse and validate RGB values
	char *ptr = color_str;
	r = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
		r = r * 10 + (*ptr++ - '0');
	if (*ptr != ',' || r > 255) return (0);
	ptr++;
	
	g = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
		g = g * 10 + (*ptr++ - '0');
	if (*ptr != ',' || g > 255) return (0);
	ptr++;
	
	b = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
		b = b * 10 + (*ptr++ - '0');
	if (b > 255) return (0);
	
	return (1);
}

int	validate_parsing_completeness(t_map *map)
{
	int	errors = 0;

	if (!map->no_texture)
	{
		printf("Error: Missing NO (North) texture\n");
		errors++;
	}
	if (!map->so_texture)
	{
		printf("Error: Missing SO (South) texture\n");
		errors++;
	}
	if (!map->we_texture)
	{
		printf("Error: Missing WE (West) texture\n");
		errors++;
	}
	if (!map->ea_texture)
	{
		printf("Error: Missing EA (East) texture\n");
		errors++;
	}
	if (!map->floor_color)
	{
		printf("Error: Missing F (Floor) color\n");
		errors++;
	}
	else if (!validate_color_format(map->floor_color))
	{
		printf("Error: Invalid floor color format: %s\n", map->floor_color);
		errors++;
	}
	if (!map->ceiling_color)
	{
		printf("Error: Missing C (Ceiling) color\n");
		errors++;
	}
	else if (!validate_color_format(map->ceiling_color))
	{
		printf("Error: Invalid ceiling color format: %s\n", map->ceiling_color);
		errors++;
	}
	
	return (errors == 0);
}

int	parse_map_config(t_map *map)
{
	int		i;
	char	*line;

	if (!map || !map->map)
		return (0);
	
	i = 0;
	while (map->map[i])
	{
		line = skip_spaces(map->map[i]);
		
		// Skip empty lines
		if (!*line || *line == '\n')
		{
			i++;
			continue;
		}
		
		// Parse texture identifiers (with duplicate check)
		if (ft_strncmp(line, "NO ", 3) == 0)
		{
			if (map->no_texture)
			{
				printf("Warning: Duplicate NO texture found\n");
				free(map->no_texture);
			}
			map->no_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "SO ", 3) == 0)
		{
			if (map->so_texture)
			{
				printf("Warning: Duplicate SO texture found\n");
				free(map->so_texture);
			}
			map->so_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "WE ", 3) == 0)
		{
			if (map->we_texture)
			{
				printf("Warning: Duplicate WE texture found\n");
				free(map->we_texture);
			}
			map->we_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "EA ", 3) == 0)
		{
			if (map->ea_texture)
			{
				printf("Warning: Duplicate EA texture found\n");
				free(map->ea_texture);
			}
			map->ea_texture = extract_path(line);
		}
		else if (ft_strncmp(line, "F ", 2) == 0)
		{
			if (map->floor_color)
			{
				printf("Warning: Duplicate F color found\n");
				free(map->floor_color);
			}
			map->floor_color = extract_path(line);
		}
		else if (ft_strncmp(line, "C ", 2) == 0)
		{
			if (map->ceiling_color)
			{
				printf("Warning: Duplicate C color found\n");
				free(map->ceiling_color);
			}
			map->ceiling_color = extract_path(line);
		}
		else if (*line == '1' || *line == '0')
		{
			// This is the start of the actual map
			break;
		}
		
		i++;
	}
	
	return (1);
}

static void cleanup_map(t_map *map)
{
    if (!map)
        return;

    if (map->no_texture)
        free(map->no_texture);
    if (map->so_texture)
        free(map->so_texture);
    if (map->we_texture)
        free(map->we_texture);
    if (map->ea_texture)
        free(map->ea_texture);
    if (map->floor_color)
        free(map->floor_color);
    if (map->ceiling_color)
        free(map->ceiling_color);

    if (map->map)
    {
        for (int i = 0; i < map->height && map->map[i]; i++)
        {
            free(map->map[i]);
        }
        free(map->map);
    }


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
int	check_and_open_file(char *filename, t_map *map)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen1(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	if (filename[0] == '.' && len == 4)
		return (0);
    map->map_fd = open(filename, O_RDONLY);
    if (map->map_fd < 0)
		return (0);
    return (1);
}

int	read_file(t_map *map, char *filename)
{
	char	*line;
	char	**file_content;
	int		line_count;
	int		i;

	if (!map || !filename || map->map_fd < 0)
		return (0);
	
	line_count = 0;
	while ((line = get_next_line(map->map_fd)))
	{
		line_count++;
		free(line);
	}
	close(map->map_fd);
	
	if (line_count == 0)
		return (0);
	

	map->map_fd = open(filename, O_RDONLY);
	if (map->map_fd < 0)
		return (0);
	
	file_content = malloc(sizeof(char *) * (line_count + 1));
	if (!file_content)
	{
		close(map->map_fd);
		return (0);
	}
	
	i = 0;
	while ((line = get_next_line(map->map_fd)))
	{
		file_content[i] = line;
		i++;
	}
	file_content[i] = NULL;
	
	close(map->map_fd);
	map->map = file_content;
	
	return (1);
}

int	main(int ac, char **av)
{
	t_map	*map;

	if (ac != 2)
	{
		printf("Error\nUsage: ./cub3d <map.cub>\n");
		return (1);
	}
	
	map = malloc(sizeof(t_map));
	if (!map)
	{
		printf("Error\nMemory allocation failed\n");
		return (1);
	}
	

	map->map = NULL;
	map->map_fd = -1;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->floor_color = NULL;
	map->ceiling_color = NULL;
	
	if (!check_and_open_file(av[1], map))
	{
		printf("Error\nInvalid file format. Please provide a .cub file\n");
		free(map);
		return (1);
	}
	
	if (!read_file(map, av[1]))
	{
		printf("Error\nFailed to read file\n");
		cleanup_map(map);
		return (1);
	}
	
	if (!parse_map_config(map))
	{
		printf("Error\nFailed to parse map configuration\n");
		cleanup_map(map);
		return (1);
	}
	
	// Validate that all required elements are present and valid
	if (!validate_parsing_completeness(map))
	{
		printf("Error\nMap configuration is incomplete or invalid\n");
		cleanup_map(map);
		return (1);
	}
	
	printf("NO: %s\n", map->no_texture ? map->no_texture : "NULL");
	printf("SO: %s\n", map->so_texture ? map->so_texture : "NULL");
	printf("WE: %s\n", map->we_texture ? map->we_texture : "NULL");
	printf("EA: %s\n", map->ea_texture ? map->ea_texture : "NULL");
	printf("F: %s\n", map->floor_color ? map->floor_color : "NULL");
	printf("C: %s\n", map->ceiling_color ? map->ceiling_color : "NULL");
	
	load_game(map);
	cleanup_map(map);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.colors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/17 21:41:50 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	check_color_chars(char *color_str, int *commas, int *num_count)
{
	int	i;

	i = 0;
	*commas = 0;
	*num_count = 0;
	while (color_str[i])
	{
		if (color_str[i] == ',')
			(*commas)++;
		else if (color_str[i] >= '0' && color_str[i] <= '9')
			(*num_count)++;
		else if (color_str[i] != ' ' && color_str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

char	*parse_rgb_value(char *ptr, int *value, int *has_digits)
{
	*value = 0;
	*has_digits = 0;
	while (*ptr && *ptr >= '0' && *ptr <= '9')
	{
		*value = *value * 10 + (*ptr++ - '0');
		*has_digits = 1;
	}
	return (ptr);
}
int	validate_color_format(char *color_str)
{
	int		commas;
	int		num_count;
	int		value;
	int		has_digits;
	char	*ptr;

	if (!color_str || !check_color_chars(color_str, &commas, &num_count))
		return (0);
	if (commas != 2 || num_count < 3)
		return (0);
	ptr = skip_spaces(color_str);
	ptr = parse_rgb_value(ptr, &value, &has_digits);
	if (!has_digits || value > 255)
		return (0);
	ptr = skip_to_comma(ptr);
	if (!ptr)
		return (0);
	ptr = parse_rgb_value(skip_spaces(ptr), &value, &has_digits);
	if (!has_digits || value > 255)
		return (0);
	ptr = skip_to_comma(ptr);
	if (!ptr)
		return (0);
	ptr = parse_rgb_value(skip_spaces(ptr), &value, &has_digits);
	if (!has_digits || value > 255)
		return (0);
	return (1);
}
int	validate_color(char *color, char *name, char *type)
{
	if (!color)
	{
		printf("Error: Missing %s (%s) color\n", name, type);
		return (0);
	}
	if (!validate_color_format(color))
	{
		printf("Error: Invalid %s color format: %s\n", type, color);
		return (0);
	}
	return (1);
}
int	validate_all_colors(t_map *map)
{
	int	valid;

	valid = 1;
	if (!validate_color(map->floor_color, "F", "Floor"))
		valid = 0;
	if (!validate_color(map->ceiling_color, "C", "Ceiling"))
		valid = 0;
	return (valid);
}

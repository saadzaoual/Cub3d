/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/15 00:00:00 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../header/the_lo3ba.h"

int		ft_strlen1(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*skip_spaces(char *str);
char	*extract_path(char *line);
int		validate_color_format(char *color_str);
int		validate_parsing_completeness(t_map *map);
int		parse_map_config(t_map *map);
int		check_map_borders(t_map *map);
int		validate_and_set_player(t_map *map);
void	cleanup_map(t_map *map);
int		check_map_closed(t_map *map, int start_x, int start_y);
char	get_map_char(t_map *map, int x, int y);

#endif

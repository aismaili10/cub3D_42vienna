/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle_color2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:09:58 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/06 13:45:04 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	rgb_valid(char *rgb)
{
	int	j;

	j = 0;
	while (rgb[j] && rgb[j] != '\n')
	{
		if (j == 0 && ft_strchr("+-", rgb[j]))
		{
			j++;
			continue ;
		}
		if (!ft_isdigit(rgb[j]))
		{
			return (false);
		}
		j++;
	}
	if (rgb[0] == '\n')
		return (false);
	return (true);
}

static int	inv_clr_specifier(char **str_rgb)
{
	int	i;

	i = 0;
	if (!str_rgb)
		return (true);
	while (str_rgb[i])
	{
		if (ft_strlen(rm_zero(str_rgb[i])) > 4)
		{
			free_str_array(&str_rgb);
			return (write(2, RD"ISSUE: RGB Range\n"CR, 29), true);
		}
		if (!rgb_valid(str_rgb[i]))
		{
			return (free_str_array(&str_rgb), true);
		}
		i++;
	}
	return (false);
}

int	comma_check(char *str)
{
	int	i;
	int	comma_counter;

	i = 0;
	comma_counter = 0;
	while (str[i])
	{
		if (str[i] == ',')
			comma_counter++;
		if (str[i] == ',' && str[i + 1] == ',')
			return (write(2, RD"ISSUE: Consecutive Commas\n"
					CR, 38), INV_MAP);
		i++;
	}
	if (comma_counter != 2)
		return (write(2, RD"ISSUE: NUM of Commas\n"
				CR, 33), INV_MAP);
	if (str[0] == ',' || str[ft_strlen(str) - 1] == ',')
		return (write(2, RD"ISSUE: Consecutive Commas\n"
				CR, 38), INV_MAP);
	return (SUCCESS);
}

int	init_u_map_clr(t_map *u_map)
{
	char	**str_rgb;
	int		rgb[3];
	int		i;

	errno = 0;
	if (comma_check(u_map->spl_ln[1]))
		return (write(2, RD"RGB Invalid Map\n"CR, 28), INV_MAP);
	str_rgb = ft_split(u_map->spl_ln[1], ',');
	if (!str_rgb && errno)
		return (perror("malloc"), SYS_FAIL);
	if (str_ary_len(str_rgb) != 3 || inv_clr_specifier(str_rgb))
		return (write(2, RD"RGB Invalid Map\n"CR, 28), INV_MAP);
	i = -1;
	while (++i < 3)
		rgb[i] = ft_atoi(str_rgb[i]);
	free_str_array(&str_rgb);
	if (inv_range(rgb))
		return (write(2, RD"RGB Inval Range\n"CR, 28), INV_MAP);
	i = ft_strlen(u_map->spl_ln[0]);
	if (!ft_strncmp(u_map->spl_ln[0], "C", i))
		u_map->c_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	if (!ft_strncmp(u_map->spl_ln[0], "F", i))
		u_map->f_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	u_map->id_ed = true;
	return (SUCCESS);
}

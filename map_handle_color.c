/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:05:58 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/04 13:47:47 by aismaili         ###   ########.fr       */
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
			//printf("rgb: -%s- --> rgb[%i]: -%c-\n", rgb, j, rgb[j]);
			return (false);
		}
		j++;
	}
	if (rgb[0] == '\n')
		return (false);
	return (true);
}

char	*rm_zero(char *num)
{
	int	i;

	i = 0;
	if (ft_strchr("+-", num[i]))
		i++;
	while (num[i] == '0' && i < (int)ft_strlen(num) - 1)
		i++;
	return (&num[i]);
}

static int	inv_clr_specifier(char **str_rgb)
{
	int	i;

	i = 0;
	while (str_rgb[i])
	{
		if (ft_strlen(rm_zero(str_rgb[i])) > 4)
		{
			//printf("bigger then 3\n%zu\n-%s-\n", ft_strlen(rm_zero(str_rgb[i])), str_rgb[i]);
			free_str_array(&str_rgb);	
			return (write(2, COLOR_RED"ISSUE: RGB Range\n"COLOR_RESET, 29),true);
		}
		if (!rgb_valid(str_rgb[i]))
		{
			//printf("inv_clr_specifier(): str_rgb[%i]: %s\n", i, str_rgb[i]);
			return (free_str_array(&str_rgb), true);
		}
		i++;
	}
	return (false);
}

int	inv_range(int *rgb)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (rgb[i] < 0 || rgb[i] > 255)
			return (true);
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
			return (write(2, COLOR_RED"ISSUE: Consecutive Commas\n"
				COLOR_RESET, 38), INV_MAP);
		i++;
	}
	if (comma_counter != 2)
		return (write(2, COLOR_RED"ISSUE: NUM of Commas\n"COLOR_RESET, 33), INV_MAP);
	if (str[0] == ',' || str[ft_strlen(str) - 1] == ',')
		return (write(2, COLOR_RED"ISSUE: Consecutive Commas\n"COLOR_RESET, 38), INV_MAP);
	return (SUCCESS);
}

int	init_u_map_clr(t_map *u_map)
{
	char	**str_rgb;
	int		rgb[3];
	int		i;

	errno = 0;
	//printf("in init_mapclr: %s\n", u_map->splited_line[1]);
	if (comma_check(u_map->splited_line[1]))
		return (write(2, COLOR_RED"RGB Invalid Map\n"COLOR_RESET, 28), INV_MAP);
	str_rgb = ft_split(u_map->splited_line[1], ',');
	if (!str_rgb && errno)
		return (perror("malloc"), SYS_FAIL);
	/* i = -1;
	while (++i < 3)
		printf("%s\n", str_rgb[i]); */
	if (str_ary_len(str_rgb) != 3 || inv_clr_specifier(str_rgb))
		return (write(2, COLOR_RED"RGB Invalid Map\n"COLOR_RESET, 28), INV_MAP);
	/* if (inv_clr_specifier(str_rgb))
		return (write(2, COLOR_RED"RGB Invalid Map\n"COLOR_RESET, 28), INV_MAP); */
	i = -1;
	while (++i < 3)
		rgb[i] = ft_atoi(str_rgb[i]); // handle overflow!!
	free_str_array(&str_rgb);
	if (inv_range(rgb))
		return (write(2, COLOR_RED"RGB Invalid Range\n"COLOR_RESET, 30), INV_MAP);
	i = ft_strlen(u_map->splited_line[0]);
	if (!ft_strncmp(u_map->splited_line[0], "C", i))
		u_map->c_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2]; // gotta understand how exactly that works
	else if (!ft_strncmp(u_map->splited_line[0], "F", i))
		u_map->f_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	u_map->id_ed = true;
	return (SUCCESS);
}

int	is_color(char *id, t_map *u_map)
{
	if (!ft_strncmp(id, "C", 2) && u_map->c_color == -1)
		return (true);
	else if (!ft_strncmp(id, "C", 2) && u_map->c_color != -1)
		return (INV_MAP);
	if (!ft_strncmp(id, "F", 2) && u_map->f_color == -1)
		return (true);
	else if (!ft_strncmp(id, "F", 2) && u_map->f_color != -1)
		return (INV_MAP);
	return (false);
}

int	handle_color(t_main *cub)
{
	int	ret;

	ret = is_color(cub->u_map.splited_line[0], &cub->u_map);
	if (ret == INV_MAP)
	{
		return (write(2, COLOR_RED"C/F Color Dublicate Encountered\n"COLOR_RESET, 44), INV_MAP);
	}
	if (ret)
	{
		return (init_u_map_clr(&cub->u_map));
	}
	return (SUCCESS);
}

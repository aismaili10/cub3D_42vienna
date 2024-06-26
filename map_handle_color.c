/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:05:58 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/26 18:22:45 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	rgb_valid(char *rgb)
{
	int	j;

	j = 0;
	while (rgb[j] && rgb[j] != '\n')
	{
		if (!ft_isdigit(rgb[j]))
		{
			//printf("rgb: %s --> rgb[%i]: %c\n", rgb, j, rgb[j]);
			return (false);
		}
		j++;
	}
	return (true);
}

static int	inv_clr_specifier(char **str_rgb)
{
	int	i;

	i = 0;
	while (str_rgb[i])
	{
		if (!rgb_valid(str_rgb[i]))
		{
			//printf("inv_clr_specifier(): str_rgb[%i]: %s\n", i, str_rgb[i]);
			return (true);
		}
		i++;
	}
	return (false);
}

int	init_u_map_clr(t_map *u_map)
{
	char	**str_rgb;
	int		rgb[3];
	int		i;

	errno = 0;
	//printf("in init_mapclr: %s\n", u_map->splited_line[1]);
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

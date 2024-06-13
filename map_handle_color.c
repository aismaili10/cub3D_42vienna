/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:05:58 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/13 14:46:42 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_u_map_clr(char *line, t_map *u_map)
{
	char	**str_rgb;
	int		rgb[3];
	int		i;

	(void)line;
	errno = 0;
	str_rgb = ft_split(u_map->splited_line[1], ',');
	if (!str_rgb && errno)
		return (perror("malloc"), SYS_FAIL);
	if (str_ary_len(str_rgb) != 3)
		return (write(2, COLOR_RED"RGB Invalid Map\n"COLOR_RESET, 28), INV_MAP);
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

int	handle_color(char *line, t_main *cub)
{
	int	ret;

	ret = is_color(cub->u_map.splited_line[0], &cub->u_map);
	if (ret == INV_MAP)
	{
		return (write(2, COLOR_RED"C/F Color Dublicate Encountered\n"COLOR_RESET, 44), INV_MAP);
	}
	if (ret)
	{
		return (init_u_map_clr(line, &cub->u_map));
	}
	return (SUCCESS);
}
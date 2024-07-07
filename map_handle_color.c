/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:05:58 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/07 10:06:00 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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

	ret = is_color(cub->u_map.spl_ln[0], &cub->u_map);
	if (ret == INV_MAP)
	{
		return (write(2, RD"Error\nC/F Color Dublicate Encountered\n"
				CR, 50), INV_MAP);
	}
	if (ret)
	{
		return (init_u_map_clr(&cub->u_map));
	}
	return (SUCCESS);
}

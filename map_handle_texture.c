/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle_texture.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:08:12 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/07 10:06:53 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// sp_line is u_map.spl_ln
int	init_u_map_txts(char **sp_line, t_map *u_map)
{
	int		len;
	char	*tmp;

	tmp = ft_strtrim(sp_line[1], " \n");
	if (!tmp)
		return (SYS_FAIL);
	len = ft_strlen(sp_line[0]);
	if (!ft_strncmp(sp_line[0], "NO", len))
		u_map->no = tmp;
	else if (!ft_strncmp(sp_line[0], "SO", len))
		u_map->so = tmp;
	else if (!ft_strncmp(sp_line[0], "WE", len))
		u_map->we = tmp;
	else if (!ft_strncmp(sp_line[0], "EA", len))
		u_map->ea = tmp;
	else
		return (free(tmp), INV_MAP);
	u_map->id_ed = true;
	return (SUCCESS);
}

int	is_texture(char *type, t_map *u_map)
{
	if (!ft_strncmp(type, "NO", 3) && !u_map->no)
		return (true);
	else if (!ft_strncmp(type, "NO", 3) && u_map->no)
		return (INV_MAP);
	if (!ft_strncmp(type, "SO", 3) && !u_map->so)
		return (true);
	else if (!ft_strncmp(type, "SO", 3) && u_map->so)
		return (INV_MAP);
	if (!ft_strncmp(type, "WE", 3) && !u_map->we)
		return (true);
	else if (!ft_strncmp(type, "WE", 3) && u_map->we)
		return (INV_MAP);
	if (!ft_strncmp(type, "EA", 3) && !u_map->ea)
		return (true);
	else if (!ft_strncmp(type, "EA", 3) && u_map->ea)
		return (INV_MAP);
	return (false);
}

int	handle_texture(t_main *cub)
{
	int	ret;

	if (cub->u_map.id_ed)
		return (SUCCESS);
	ret = is_texture(cub->u_map.spl_ln[0], &cub->u_map);
	if (ret == INV_MAP)
	{
		write(2, RD"Error\nWALL Texture Dublicate Encountered\n"CR, 53);
		return (INV_MAP);
	}
	if (ret == true)
		return (init_u_map_txts(cub->u_map.spl_ln, &cub->u_map));
	return (SUCCESS);
}

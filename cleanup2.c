/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:27:17 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 13:28:02 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_textures(t_map *u_map)
{
	if (u_map->no)
		free(u_map->no);
	if (u_map->so)
		free(u_map->so);
	if (u_map->we)
		free(u_map->we);
	if (u_map->ea)
		free(u_map->ea);
}

void	free_map(char **map)
{
	int	i;

	i = -1;
	if (!map)
		return ;
	while (map[++i])
		free(map[i]);
	free(map);
}

void	free_map_elements(t_main *cub)
{
	if (cub->u_map.fd != -1)
		close(cub->u_map.fd);
	free_textures(&cub->u_map);
	if (cub->u_map.joined_lines)
		free(cub->u_map.joined_lines);
	free_str_array(&cub->u_map.spl_ln);
	free_map(cub->u_map.map);
}

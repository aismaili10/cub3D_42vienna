/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:27 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/13 15:56:49 by aismaili         ###   ########.fr       */
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
	free_str_array(&cub->u_map.splited_line);
	free_map(cub->u_map.map);
}

void	cleanup(t_main *cub, int stage)
{
	(void)cub;
	(void)stage;
	printf("in cleanup; stage: %i\n", stage);
	if (stage == 10)
		print_map_elements(&cub->u_map);
	if (stage == 1)
	{
		get_next_line(cub->u_map.fd, 1);
	}
	free_map_elements(cub);
	if (stage == 10)
		printf(COLOR_GREEN"map is valid\n"COLOR_RESET);
	exit(1);
	/*if (stage == 1)
		cleanup_1();
	if (stage == 2)
		cleanup_2();*/
	// ...
}

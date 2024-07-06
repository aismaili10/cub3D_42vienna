/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:04:52 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/06 14:22:51 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_player_pos(t_player *player, int x, int y)
{
	player->pos_x = x + 0.5;
	player->pos_y = y + 0.5;
}

void	init_w_e(t_player *player, int index)
{
	if (index == 1)
	{
		player->dir_x = -1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = -0.66;
	}
	if (index == 2)
	{
		player->dir_x = 1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = 0.66;
	}
}

void	init_s_n(t_player *player, int index)
{
	if (index == 3)
	{
		player->dir_x = 0;
		player->dir_y = 1;
		player->plane_x = -0.66;
		player->plane_y = 0;
	}
	if (index == 4)
	{
		player->dir_x = 0;
		player->dir_y = -1;
		player->plane_x = 0.66;
		player->plane_y = 0;
	}
}

void	init_player(t_player *player, t_map u_map)
{
	int	x;
	int	y;

	y = -1;
	while (u_map.map[++y])
	{
		x = -1;
		while (u_map.map[y][++x])
		{
			if (u_map.map[y][x] == 'N' || u_map.map[y][x] == 'S'
			|| u_map.map[y][x] == 'E' || u_map.map[y][x] == 'W')
			{
				init_player_pos(player, x, y);
				if (u_map.map[y][x] == 'W')
					init_w_e(player, 1);
				if (u_map.map[y][x] == 'E')
					init_w_e(player, 2);
				if (u_map.map[y][x] == 'S')
					init_s_n(player, 3);
				if (u_map.map[y][x] == 'N')
					init_s_n(player, 4);
				return ;
			}
		}
	}
}

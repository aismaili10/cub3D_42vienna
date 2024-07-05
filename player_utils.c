/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:04:52 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 12:07:01 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_player_pos(t_player *player, int x, int y)
{
	player->posX = x + 0.5;
	player->posY = y + 0.5;
}

void	init_w_e(t_player *player, int index)
{
	if (index == 1)
	{
		player->dirX = -1;
		player->dirY = 0;
		player->planeX = 0;
		player->planeY = -0.66;
	}
	if (index == 2)
	{
		player->dirX = 1;
		player->dirY = 0;
		player->planeX = 0;
		player->planeY = 0.66;
	}
}

void	init_s_n(t_player *player, int index)
{
	if (index == 3)
	{
		player->dirX = 0;
		player->dirY = 1;
		player->planeX = -0.66;
		player->planeY = 0;
	}
	if (index == 4)
	{
		player->dirX = 0;
		player->dirY = -1;
		player->planeX = 0.66;
		player->planeY = 0;
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
				if (u_map.map[y][x] == 'W') // west and east might be mixed up
					init_w_e(player, 1);
				if (u_map.map[y][x] == 'E')
					init_w_e(player, 2);
				if (u_map.map[y][x] == 'S') // north and south might be mixed up
					init_s_n(player, 3);
				if (u_map.map[y][x] == 'N')
					init_s_n(player, 4);
				return ;
			}
		}
	}
}

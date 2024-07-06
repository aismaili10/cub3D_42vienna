/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_cal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:15:35 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/06 14:28:32 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	cal_viewport_bound_end(t_minimap *mini, t_map *u_map)
{
	if (mini->end_x >= u_map->width)
	{
		mini->end_x = u_map->width - 1;
		if (u_map->width <= MINIMAP_WIDTH)
			mini->start_x = 0;
		else
			mini->start_x = mini->end_x - MINIMAP_WIDTH;
	}
	if (mini->end_y >= u_map->height)
	{
		mini->end_y = u_map->height - 1;
		if (u_map->height <= MINIMAP_HEIGHT)
			mini->start_y = 0;
		else
			mini->start_y = mini->end_y - MINIMAP_HEIGHT;
	}
}

void	cal_viewport_bound(t_minimap *mini, t_map *u_map)
{
	if (mini->start_x < 0)
	{
		mini->start_x = 0;
		mini->end_x = MINIMAP_WIDTH;
	}
	if (mini->start_y < 0)
	{
		mini->start_y = 0;
		mini->end_y = MINIMAP_HEIGHT;
	}
	cal_viewport_bound_end(mini, u_map);
}

void	cal_viewport(t_minimap *mini, t_player *player, t_map *u_map)
{
	mini->player_x = (int)player->pos_x;
	mini->player_y = (int)player->pos_y;
	mini->start_x = mini->player_x - MINIMAP_WIDTH / 2;
	mini->start_y = mini->player_y - MINIMAP_HEIGHT / 2;
	mini->end_x = mini->player_x + MINIMAP_WIDTH / 2;
	mini->end_y = mini->player_y + MINIMAP_HEIGHT / 2;
	cal_viewport_bound(mini, u_map);
}

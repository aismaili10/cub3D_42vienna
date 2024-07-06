/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 20:49:06 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/06 14:21:49 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_square(t_main *cub, int x, int y, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < MINIMAP_SCALE)
	{
		j = 0;
		while (j < MINIMAP_SCALE)
		{
			pixel_put(&cub->mlx_img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}

void	draw_circle(t_main *cub, int cx, int cy, int radius)
{
	int	x;
	int	y;
	int	color;

	y = -radius;
	color = 0xFF0000;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				pixel_put(&cub->mlx_img, cx + x, cy + y, color);
			x++;
		}
		y++;
	}
}

void	draw_minimap_grid(t_main *cub, t_minimap *mini)
{
	int	y;
	int	x;
	int	color;

	y = mini->start_y;
	while (y <= mini->end_y)
	{
		x = mini->start_x;
		while (x <= mini->end_x)
		{
			if (cub->u_map.map[y][x] == '1')
				color = 0x008888;
			else
				color = 0xFFFF00;
			draw_square(cub, (x - mini->start_x) * MINIMAP_SCALE,
				(y - mini->start_y) * MINIMAP_SCALE, color);
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_main *cub)
{
	t_minimap	mini;
	int			px;
	int			py;

	cal_viewport(&mini, cub->player, &cub->u_map);
	draw_minimap_grid(cub, &mini);
	px = (cub->player->pos_x - mini.start_x) * MINIMAP_SCALE;
	py = (cub->player->pos_y - mini.start_y) * MINIMAP_SCALE;
	draw_circle(cub, px, py, PLAYER_RADIUS);
}

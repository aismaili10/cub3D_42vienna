/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 20:49:06 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/05 15:43:08 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void draw_square(t_main *cub, int x, int y, int color)
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

void draw_circle(t_main *cub, int cx, int cy, int radius, int color)
{
	int	x;
	int	y;

	y = -radius;
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

void	cal_viewport(t_minimap *mini, t_player *player, t_map *u_map)
{
	mini->player_x = (int)player->pos_x;
	mini->player_y = (int)player->pos_y;
	mini->start_x = mini->player_x - MINIMAP_WIDTH / 2;
	mini->start_y = mini->player_y - MINIMAP_HEIGHT / 2;
	mini->end_x = mini->player_x + MINIMAP_WIDTH / 2;
	mini->end_y = mini->player_y + MINIMAP_HEIGHT / 2;
	printf("before: start_x: %i; start_y: %i; end_x: %i; end_y: %i\n", mini->start_x, mini->start_y, mini->end_x, mini->end_y);
	/* if (mini->start_x < 0)
		mini->start_x = 0;
	if (mini->start_y < 0)
		mini->start_y = 0;
	if (mini->end_x >= u_map->width)
		mini->end_x = u_map->width - 1;
	if (mini->end_y >= u_map->height)
		mini->end_y = u_map->height - 1; */
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
    if (mini->end_x >= u_map->width)
    {
        mini->end_x = u_map->width - 1;
		if (u_map->width <= MINIMAP_WIDTH)
	        mini->start_x = 0 /* mini->end_x - u_map->width */;
		else
	        mini->start_x = mini->end_x - MINIMAP_WIDTH;
    }
    if (mini->end_y >= u_map->height)
    {
        mini->end_y = u_map->height - 1;
		if (u_map->height <= MINIMAP_HEIGHT)
	        mini->start_y = 0 /* mini->end_y - u_map->height */;
		else
	        mini->start_y = mini->end_y - MINIMAP_HEIGHT;
    }
	printf("after: start_x: %i; start_y: %i; end_x: %i; end_y: %i\n", mini->start_x, mini->start_y, mini->end_x, mini->end_y);
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

void draw_minimap(t_main *cub)
{
	t_minimap	mini;

	cal_viewport(&mini, cub->player, &cub->u_map); // Calculate viewport boundaries
	draw_minimap_grid(cub, &mini);
	px = (cub->player->pos_x - mini.start_x) * MINIMAP_SCALE;
	py = (cub->player->pos_y - mini.start_y) * MINIMAP_SCALE;
	draw_circle(cub, px, py, PLAYER_RADIUS, 0xFF0000);
}

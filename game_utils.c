/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:52:47 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/04 15:01:43 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int close_window(t_main *cub)
{
    cleanup(cub, 3);
    return (0);
}

void	pixel_put(t_img *mlx_img, int x, int y, int color)
{
	char	*dst;

	dst = mlx_img->addr + (y * mlx_img->line_length + x * (mlx_img->bpp / 8));
	*(unsigned int*)dst = color;
}

int render_background(t_main *cub)
{
	int x;
	int y;

	y = 0;
	if (cub->mlx_ptr)
	{
		while (y < WIN_HEIGHT)
		{
			x = 0;
			while (x < WIN_WIDTH)
			{
				if (y < WIN_HEIGHT / 2)
					pixel_put(&cub->mlx_img, x, y, cub->u_map.c_color); // cub->u_map.f_color
				else
					pixel_put(&cub->mlx_img, x, y, cub->u_map.f_color); // cub->u_map.f_color
				x++;
			}
			y++;
		}
	}
	return (SUCCESS);
}

void	init_player(t_player *player, t_main *cub, t_map u_map)
{
	size_t x; // why x and y different types?
	int y;
	
	y = -1;
	x = 0;
	cub->player = player;
	while (u_map.map[++y])
	{
		x = -1;
		while (u_map.map[y][++x])
		{
			if (u_map.map[y][x] == 'N' || u_map.map[y][x] == 'S' || u_map.map[y][x] == 'E' || u_map.map[y][x] == 'W')
			{
				player->posX = x + 0.5; // get player position
				player->posY = y + 0.5;
				if (u_map.map[y][x] == 'W') // west and east might be mixed up
				{
					player->dirX = -1;
					player->dirY = 0;
					player->planeX = 0; // what is plane x and plane y for?
					player->planeY = -0.66;
				}
				if (u_map.map[y][x] == 'E')
				{
					player->dirX = 1;
					player->dirY = 0;
					player->planeX = 0;
					player->planeY = 0.66;
				}
				if (u_map.map[y][x] == 'S') // north and south might be mixed up
				{
					player->dirX = 0;
					player->dirY = 1;
					player->planeX = -0.66;
					player->planeY = 0;
				}
				if (u_map.map[y][x] == 'N')
				{
					player->dirX = 0;
					player->dirY = -1;
					player->planeX = 0.66;
					player->planeY = 0;
				}
                return;
			}
		}
	}
}
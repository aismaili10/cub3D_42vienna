/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:26:51 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 12:29:08 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	get_draw_values(t_player *p, t_render *r)
{
	if (r->side == 0)
		r->perpWallDist = (r->mapX - p->posX + (1 - r->stepX) / 2) / p->rayDirX;
	else
		r->perpWallDist = (r->mapY - p->posY + (1 - r->stepY) / 2) / p->rayDirY;
	r->lineHeight = (int)(WIN_HEIGHT / r->perpWallDist);
	r->drawStart = -r->lineHeight / 2 + WIN_HEIGHT / 2;
	if (r->drawStart < 0)
		r->drawStart = 0;
	r->drawEnd = r->lineHeight / 2 + WIN_HEIGHT / 2;
	if (r->drawEnd >= WIN_HEIGHT || r->drawEnd < 0)
		r->drawEnd = WIN_HEIGHT - 1;
}

void	get_ray_direction(t_player *player, int x)
{
	double	cam_x;

	cam_x = 2 * x / (double)WIN_WIDTH - 1;
	player->rayDirX = player->dirX + player->planeX * cam_x;
	player->rayDirY = player->dirY + player->planeY * cam_x;
}

void	get_distances(t_player *player, t_render *r)
{
	r->mapX = (int)player->posX;
	r->mapY = (int)player->posY;
	r->deltaDistX = fabs(1 / player->rayDirX);
	r->deltaDistY = fabs(1 / player->rayDirY);
	if (player->rayDirX < 0)
	{
		r->stepX = -1;
		r->sideDistX = (player->posX - r->mapX) * r->deltaDistX;
	}
	else
	{
		r->stepX = 1;
		r->sideDistX = (r->mapX + 1.0 - player->posX) * r->deltaDistX;
	}
	if (player->rayDirY < 0)
	{
		r->stepY = -1;
		r->sideDistY = (player->posY - r->mapY) * r->deltaDistY;
	}
	else
	{
		r->stepY = 1;
		r->sideDistY = (r->mapY + 1.0 - player->posY) * r->deltaDistY;
	}
}

void	hit_wall_loop(t_main *cub, t_render *render)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (render->sideDistX < render->sideDistY)
		{
			render->sideDistX += render->deltaDistX;
			render->mapX += render->stepX;
			render->side = 0;
		}
		else
		{
			render->sideDistY += render->deltaDistY;
			render->mapY += render->stepY;
			render->side = 1;
		}
		if (cub->u_map.map[render->mapY][render->mapX] == '1')
			hit = 1;
	}
}

void	get_texture_index(t_player *player, t_render *r)
{
	if (r->side == 0)
	{
		if (player->rayDirX > 0)
			r->texIndex = 3;
		else
			r->texIndex = 2;
	}
	else
	{
		if (player->rayDirY > 0)
			r->texIndex = 0;
		else
			r->texIndex = 1;
	}
	if (r->side == 0)
		r->wallX = player->posY + r->perpWallDist * player->rayDirY;
	else
		r->wallX = player->posX + r->perpWallDist * player->rayDirX;
	r->wallX -= floor(r->wallX);
}

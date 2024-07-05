/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:26:51 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 15:46:03 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	get_draw_values(t_player *p, t_render *r)
{
	if (r->side == 0)
		r->perp_dist_wall = (r->map_x - p->pos_x + (1 - r->step_x) / 2)
			/ p->ray_x;
	else
		r->perp_dist_wall = (r->map_y - p->pos_y + (1 - r->step_y) / 2)
			/ p->ray_y;
	r->lineheight = (int)(WIN_HEIGHT / r->perp_dist_wall);
	r->draw_start = -r->lineheight / 2 + WIN_HEIGHT / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	r->draw_end = r->lineheight / 2 + WIN_HEIGHT / 2;
	if (r->draw_end >= WIN_HEIGHT || r->draw_end < 0)
		r->draw_end = WIN_HEIGHT - 1;
}

void	get_ray_direction(t_player *player, int x)
{
	double	cam_x;

	cam_x = 2 * x / (double)WIN_WIDTH - 1;
	player->ray_x = player->dir_x + player->plane_x * cam_x;
	player->ray_y = player->dir_y + player->plane_y * cam_x;
}

void	get_distances(t_player *player, t_render *r)
{
	r->map_x = (int)player->pos_x;
	r->map_y = (int)player->pos_y;
	r->delta_x = fabs(1 / player->ray_x);
	r->delta_y = fabs(1 / player->ray_y);
	if (player->ray_x < 0)
	{
		r->step_x = -1;
		r->side_dist_x = (player->pos_x - r->map_x) * r->delta_x;
	}
	else
	{
		r->step_x = 1;
		r->side_dist_x = (r->map_x + 1.0 - player->pos_x) * r->delta_x;
	}
	if (player->ray_y < 0)
	{
		r->step_y = -1;
		r->side_dist_y = (player->pos_y - r->map_y) * r->delta_y;
	}
	else
	{
		r->step_y = 1;
		r->side_dist_y = (r->map_y + 1.0 - player->pos_y) * r->delta_y;
	}
}

void	hit_wall_loop(t_main *cub, t_render *render)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (render->side_dist_x < render->side_dist_y)
		{
			render->side_dist_x += render->delta_x;
			render->map_x += render->step_x;
			render->side = 0;
		}
		else
		{
			render->side_dist_y += render->delta_y;
			render->map_y += render->step_y;
			render->side = 1;
		}
		if (cub->u_map.map[render->map_y][render->map_x] == '1')
			hit = 1;
	}
}

void	get_texture_index(t_player *player, t_render *r)
{
	if (r->side == 0)
	{
		if (player->ray_x > 0)
			r->tex_index = 3;
		else
			r->tex_index = 2;
	}
	else
	{
		if (player->ray_y > 0)
			r->tex_index = 1;
		else
			r->tex_index = 0;
	}
	if (r->side == 0)
		r->wall_x = player->pos_y + r->perp_dist_wall * player->ray_y;
	else
		r->wall_x = player->pos_x + r->perp_dist_wall * player->ray_x;
	r->wall_x -= floor(r->wall_x);
}

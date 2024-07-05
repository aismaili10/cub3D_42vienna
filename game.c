/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 11:58:55 by aszabo           ###   ########.fr       */
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

void	draw_loop(t_main *cub, t_player *player, t_render *render, int x)
{
	int	tex_x;
	int	tex_y;
	int	color;
	int	d;
	int	y;

	tex_x = (int)(render->wallX * (double)TEX_WIDTH);
	if (render->side == 0 && player->rayDirX > 0)
		tex_x = TEX_WIDTH - tex_x - 1;
	if (render->side == 1 && player->rayDirY < 0)
		tex_x = TEX_WIDTH - tex_x - 1;
	y = render->drawStart;
	while (y < render->drawEnd)
	{
		d = y * 256 - WIN_HEIGHT * 128 + render->lineHeight * 128;
		tex_y = ((d * TEX_HEIGHT) / render->lineHeight) / 256;
		color = cub->texture_buff[render->texIndex][TEX_HEIGHT * tex_y + tex_x];
		pixel_put(&cub->mlx_img, x, y, color);
		y++;
	}
}
void	cast_rays(t_main *cub)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		get_ray_direction(cub->player, x);
		get_distances(cub->player, cub->render);
		hit_wall_loop(cub, cub->render);
		get_draw_values(cub->player, cub->render);
		get_texture_index(cub->player, cub->render);
		draw_loop(cub, cub->player, cub->render, x);
		x++;
	}
}

int render(t_main *cub)
{
	process_input(cub);  // Process input before rendering
	render_background(cub);
	cast_rays(cub);
	draw_minimap(cub); // Call minimap rendering
	mlx_put_image_to_window(cub->mlx_ptr,
		cub->win_ptr, cub->mlx_img.img_ptr, 0, 0);
	return (SUCCESS);
}

int game(t_main *cub)
{
	t_player	*player;
	t_render	*render_vals;

	player = malloc(sizeof(t_player));
	if (!player)
		return (FAILURE);
	render_vals = malloc(sizeof(t_render));
	if (!render_vals)
		return (free(player), FAILURE);
	cub->player = player;
	cub->render = render_vals;
	cub->u_map.height = str_ary_len(cub->u_map.map);
	cub->u_map.width = ft_strlen(cub->u_map.map[0]);
	cub->key_states = (t_key_states){0}; // Initialize key states
	if (handle_textures(cub) == FAILURE)
		return (FAILURE);
	init_player(cub->player, cub->u_map);
	mlx_hook(cub->win_ptr, 2, 1L << 0, key_down, cub);// Key press
    mlx_hook(cub->win_ptr, 3, 1L << 1, key_up, cub);// Key release
	mlx_loop_hook(cub->mlx_ptr, &render, cub);
	// mlx_hook(cub->win_ptr, 2, 1L << 0, key_handle, cub);
	//mlx_hook(cub->win_ptr, DestroyNotify, KeyReleaseMask, mlx_loop_end, cub->mlx_ptr);
	mlx_hook(cub->win_ptr, 17, 0L, close_window, cub);
	mlx_loop(cub->mlx_ptr);
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/04 17:43:53 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/* void verLine(t_main *cub, int x, int drawStart, int drawEnd, int color)
{
	int y;

	//printf("drawStart: %d, drawEnd: %d\n", drawStart, drawEnd);
	y = drawStart;
	while (y < drawEnd)
	{
		*(unsigned int *)(cub->mlx_img.addr + (y * cub->mlx_img.line_length + x * (cub->mlx_img.bpp / 8))) = color;
		y++;
	}
} */

/* int	key_handle(int keycode, t_main *cub)
{
	if (keycode == ESC)
		cleanup(cub, 3);
	else if (keycode == KEY_LEFT)
		rotate_player(cub->player, -ROT_SPEED);
	else if (keycode == KEY_RIGHT)
		rotate_player(cub->player, ROT_SPEED);
	else if (keycode == KEY_W)
		move_player(cub, cub->player->dirX * MOVE_SPEED, cub->player->dirY * MOVE_SPEED);
	else if (keycode == KEY_S)
		move_player(cub, -cub->player->dirX * MOVE_SPEED, -cub->player->dirY * MOVE_SPEED);
	else if (keycode == KEY_A)
		move_player(cub, -cub->player->dirY * MOVE_SPEED, cub->player->dirX * MOVE_SPEED);
	else if (keycode == KEY_D)
		move_player(cub, cub->player->dirY * MOVE_SPEED, -cub->player->dirX * MOVE_SPEED);
	return (SUCCESS);
} */

void	get_draw_values(t_player *player, t_render *render)
{
	if (render->side == 0)
			render->perpWallDist = (render->mapX - player->posX + (1 - render->stepX) / 2) / player->rayDirX;
		else
			render->perpWallDist = (render->mapY - player->posY + (1 - render->stepY) / 2) / player->rayDirY;

		render->lineHeight = (int)(WIN_HEIGHT / render->perpWallDist);
		render->drawStart = -render->lineHeight / 2 + WIN_HEIGHT / 2;
		if (render->drawStart < 0)
			render->drawStart = 0;
		render->drawEnd = render->lineHeight / 2 + WIN_HEIGHT / 2;
		if (render->drawEnd >= WIN_HEIGHT || render->drawEnd < 0)
			render->drawEnd = WIN_HEIGHT - 1;
}

void	get_ray_direction(t_player *player, int x)
{
	double cameraX;

	cameraX = 2 * x / (double)WIN_WIDTH - 1;
	player->rayDirX = player->dirX + player->planeX * cameraX;
	player->rayDirY = player->dirY + player->planeY * cameraX;
}

void	get_distances(t_player *player, t_render *render)
{
	render->mapX = (int)player->posX;
	render->mapY = (int)player->posY;
	render->deltaDistX = fabs(1 / player->rayDirX);
	render->deltaDistY = fabs(1 / player->rayDirY);
	if (player->rayDirX < 0)
	{
		render->stepX = -1;
		render->sideDistX = (player->posX - render->mapX) * render->deltaDistX;
	}
	else
	{
		render->stepX = 1;
		render->sideDistX = (render->mapX + 1.0 - player->posX) * render->deltaDistX;
	}
	if (player->rayDirY < 0)
	{
		render->stepY = -1;
		render->sideDistY = (player->posY - render->mapY) * render->deltaDistY;
	}
	else
	{
		render->stepY = 1;
		render->sideDistY = (render->mapY + 1.0 - player->posY) * render->deltaDistY;
	}
}

void	hit_wall_loop(t_main *cub, t_render *render)
{
	int hit;

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
			/* if (mapX < 0 || mapX >= width
				|| mapY < 0 ||  mapY >= height)
			{
				hit = 1;
				break;
			}
			else  */if (cub->u_map.map[render->mapY][render->mapX] == '1')
				hit = 1;
		}
}

void	get_texture_index(t_player *player, t_render *render)
{
	if (render->side == 0)
		{
			if (player->rayDirX > 0)
				render->texIndex = 3;
			else
				render->texIndex = 2;
		}
		else
		{
			if (player->rayDirY > 0)
				render->texIndex = 0;
			else
				render->texIndex = 1;
		}
		if (render->side == 0)
			render->wallX = player->posY + render->perpWallDist * player->rayDirY;
		else
			render->wallX = player->posX + render->perpWallDist * player->rayDirX;
		render->wallX -= floor(render->wallX);
}

void	draw_loop(t_main *cub, t_player *player, t_render *render, int x)
{
	int texX;
	int texY;
	int color;
	int d;
	int y;
	
	texX = (int)(render->wallX * (double)TEX_WIDTH);
	if (render->side == 0 && player->rayDirX > 0)
		texX = TEX_WIDTH - texX - 1;
	if (render->side == 1 && player->rayDirY < 0)
		texX = TEX_WIDTH - texX - 1;
	y = render->drawStart;
	while (y < render->drawEnd)
	{
		d = y * 256 - WIN_HEIGHT * 128 + render->lineHeight * 128;
		texY = ((d * TEX_HEIGHT) / render->lineHeight) / 256;
		color = cub->texture_buff[render->texIndex][TEX_HEIGHT * texY + texX];
		pixel_put(&cub->mlx_img, x, y, color);
		y++;
	}
}
void	cast_rays(t_main *cub)
{
	int x;

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
	// draw_minimap(cub); // Call minimap rendering
	mlx_put_image_to_window(cub->mlx_ptr, cub->win_ptr, cub->mlx_img.img_ptr, 0, 0);
	return (SUCCESS);
}

int game(t_main *cub)
{
	t_player *player;
	t_render *render_vals;

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
	init_player(cub->player, cub, cub->u_map);
	mlx_hook(cub->win_ptr, 2, 1L << 0, key_down, cub);  // Key press
    mlx_hook(cub->win_ptr, 3, 1L << 1, key_up, cub);    // Key release
	mlx_loop_hook(cub->mlx_ptr, &render, cub);
	// mlx_hook(cub->win_ptr, 2, 1L << 0, key_handle, cub);
	//mlx_hook(cub->win_ptr, DestroyNotify, KeyReleaseMask, mlx_loop_end, cub->mlx_ptr);
	mlx_hook(cub->win_ptr, 17, 0L, close_window, cub);
	mlx_loop(cub->mlx_ptr);
	return (SUCCESS);
}

/* if (hit == 0)
	continue ; */
//int color = 0x00FF00;
/* 
if (cub->u_map.map[mapY][mapX] == '1')
{
	color = 0xFF0000; // color for walls
}
else
{
	color = 0x00FF00; // color for empty space
}
if (side == 1)
	color = color / 2;	
	//printf("drawStart: %i and drawEnd: %i\n", drawStart, drawEnd);
verLine(cub, x, drawStart, drawEnd, color); */

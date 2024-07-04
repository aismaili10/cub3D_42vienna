/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/04 14:04:20 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// Array to hold the state of keys

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

void	cast_rays(t_main *cub)
{
	int x;
	double cameraX;
	double rayDirX;
	double rayDirY;
	double sideDistX;
	double sideDistY;
	double deltaDistX;
	double deltaDistY;
	double perpWallDist;
	int stepX;
	int stepY;
	int hit;
	int side;
	int mapX;
	int mapY;
	int lineHeight;
	int drawStart;
	int drawEnd;
	int height;
	int texNum;
	double wallX;
	int texX;
	int texY;
	int color;
	int d;

	int	width = (int)ft_strlen(cub->u_map.map[0]);
	height = 0;
	while (cub->u_map.map[height])
    	height++;

	x = 0;
	while (x < WIN_WIDTH)
	{
		// calculate the ray direction
		cameraX = 2 * x / (double)WIN_WIDTH - 1;
		rayDirX = cub->player->dirX + cub->player->planeX * cameraX;
		rayDirY = cub->player->dirY + cub->player->planeY * cameraX;

		mapX = (int)cub->player->posX;
		mapY = (int)cub->player->posY;

		deltaDistX = fabs(1 / rayDirX);
		deltaDistY = fabs(1 / rayDirY);

		  // Calculate delta distances
        // deltaDistX = fabs(1 / (rayDirX != 0 ? rayDirX : 1e-30)); // Avoid division by zero
        // deltaDistY = fabs(1 / (rayDirY != 0 ? rayDirY : 1e-30)); // Avoid division by zero

		// use ray direction to init stepX and stepY
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (cub->player->posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - cub->player->posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (cub->player->posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - cub->player->posY) * deltaDistY;
		}
		hit = 0;
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (mapX < 0 || mapX >= width
				|| mapY < 0 ||  mapY >= height)
			{
				hit = 1;
				//printf("OUT of BOUND with x: %i >= len: %i; y: %i >= height: %i\n", mapX, cub->u_map.width, mapY, height);
				break;
			}
			else if (cub->u_map.map[mapY][mapX] == '1')
			{
				//printf("cub->u_map.map[%i][%i]: %c\n", mapY, mapX, cub->u_map.map[mapY][mapX]);
				hit = 1;
			}
		}
		if (hit == 0)
			continue ;
		if (side == 0)
			perpWallDist = (mapX - cub->player->posX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - cub->player->posY + (1 - stepY) / 2) / rayDirY;

		lineHeight = (int)(WIN_HEIGHT / perpWallDist);
		//printf("lineHeight: %d\n", lineHeight);
		drawStart = -lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + WIN_HEIGHT / 2;
		if (drawEnd >= WIN_HEIGHT || drawEnd < 0)
			drawEnd = WIN_HEIGHT - 1;
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
		texNum = 0;
		if (side == 0)
		{
			if (rayDirX > 0)
				texNum = 3;
			else
				texNum = 2;
		}
		else
		{
			if (rayDirY > 0)
				texNum = 0;
			else
				texNum = 1;
		}
		if (side == 0)
			wallX = cub->player->posY + perpWallDist * rayDirY;
		else
			wallX = cub->player->posX + perpWallDist * rayDirX;
		wallX -= floor(wallX);
		texX = (int)(wallX * (double)TEX_WIDTH);
		if (side == 0 && rayDirX > 0)
			texX = TEX_WIDTH - texX - 1;
		if (side == 1 && rayDirY < 0)
			texX = TEX_WIDTH - texX - 1;
		int y = drawStart;
		while (y < drawEnd)
		{
			d = y * 256 - WIN_HEIGHT * 128 + lineHeight * 128;
			texY = ((d * TEX_HEIGHT) / lineHeight) / 256;
			color = cub->texture_buff[texNum][TEX_HEIGHT * texY + texX];
			pixel_put(&cub->mlx_img, x, y, color);
			y++;
		}
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

	player = malloc(sizeof(t_player));
	if (!player)
		return (FAILURE);
	cub->player = player;
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

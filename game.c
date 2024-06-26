/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/06/26 14:18:08 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/* int	close_window(t_main *cub)
{
	write(1, COLOR_YELLOW"Closing Window\n"COLOR_RESET, 27);
	cleanup(cub, 2);
	return (SUCCESS);
} */

/* int	key_handle(int keycode, t_main *cub)
{
	if (keycode == ESC)
	{
		write(1, COLOR_YELLOW"Closing Window\n"COLOR_RESET, 27);
		cleanup(cub, 2);
	}
	return (SUCCESS);
} */
int close_window(t_main *cub)
{
    cleanup(cub, 2);
    return (0);
}

void	pixel_put(t_img *mlx_img, int x, int y, int color)
{
	char	*dst;

	dst = mlx_img->addr + (y * mlx_img->line_length + x * (mlx_img->bpp / 8));
	*(unsigned int*)dst = color;
}

void verLine(t_main *cub, int x, int drawStart, int drawEnd, int color)
{
	int y;

	//printf("drawStart: %d, drawEnd: %d\n", drawStart, drawEnd);
	y = drawStart;
	while (y < drawEnd)
	{
		*(unsigned int *)(cub->mlx_img.addr + (y * cub->mlx_img.line_length + x * (cub->mlx_img.bpp / 8))) = color;
		y++;
	}
}

void	init_player(t_player *player, t_main *cub, t_map u_map)
{
	size_t x;
	int y;
	
	y = -1;
	x = 0;
	cub->player = player;
	while (u_map.map[++y])
	{
		x = -1;
		while (u_map.map[y][++x])
		{
			if (u_map.map[y][x] == 'N')
			{
				player->posX = x + 0.5;
				player->posY = y + 0.5;
				player->dirX = -1;
				player->dirY = 0;
				player->planeX = 0;
				player->planeY = 0.66;
				printf("Player initialized at position: (%f, %f)\n", player->posX, player->posY);
                return;
			}
		}
	}
}
void	move_player(t_main *cub, double moveX, double moveY)
{
	int newPosX;
	int newPosY;
	char	mapCell;
	static int i = 0;

	newPosX = (int)cub->player->posX + moveX;
	newPosY = (int)cub->player->posY + moveY;

	if (newPosX < 0 || newPosX >= (int)ft_strlen(cub->u_map.map[(int)cub->player->posY])
		|| newPosY < 0 || newPosY >= (int)ft_strlen(cub->u_map.map[(int)cub->player->posY]))
		{
			printf("Out of bounds\n");
			return ;
		}
	mapCell = cub->u_map.map[newPosY][newPosX];

	if (mapCell == '0' || mapCell == 'N') //here we also need check for N,S,E,W
	{
		cub->player->posX += moveX;
		cub->player->posY += moveY;	
	}
	else
	{
        i++;
		printf("Collision detected, movement blocked %d \n", i); // Debug print
	}
}

void	rotate_player(t_player *player, double angle)
{
	double oldDirX;
	double oldPlaneX;

	oldDirX = player->dirX;
	oldPlaneX = player->planeX;
	player->dirX = player->dirX * cos(angle) - player->dirY * sin(angle);
	player->dirY = oldDirX * sin(angle) + player->dirY * cos(angle);

	player->planeX = player->planeX * cos(angle) - player->planeY * sin(angle);
	player->planeY = oldPlaneX * sin(angle) + player->planeY * cos(angle);
}


int	key_handle(int keycode, t_main *cub)
{
	if (keycode == ESC)
		cleanup(cub, 2);
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
					pixel_put(&cub->mlx_img, x, y, 0x0EA5C0);
				else
					pixel_put(&cub->mlx_img, x, y, 0x0F7910);
				if (x < 5 || x > WIN_WIDTH - 5 || y < 5 || y > WIN_HEIGHT - 5) // not really necessary here, just blackened the edges
					pixel_put(&cub->mlx_img, x, y, 0x000000);
				x++;
			}
			y++;
		}
	}
	//mlx_put_image_to_window(cub->mlx_ptr, cub->win_ptr, cub->mlx_img.img_ptr, 0, 0);
	return (SUCCESS);
}

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

	height = 0;
	while (cub->u_map.map[height])
    	height++;

	x = 0;
	while (x < WIN_WIDTH)
	{
		cameraX = 2 * x / (double)WIN_WIDTH - 1;
		rayDirX = cub->player->dirX + cub->player->planeX * cameraX;
		rayDirY = cub->player->dirY + cub->player->planeY * cameraX;

		mapX = (int)cub->player->posX;
		mapY = (int)cub->player->posY;

		deltaDistX = fabs(1 / rayDirX);
		deltaDistY = fabs(1 / rayDirY);
		
		hit = 0;
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
			if (mapX < 0 || mapX >= (int)ft_strlen(cub->u_map.map[mapY])
				|| mapY < 0 ||  mapY >= height)
			{
				hit = 1;
				printf("OUT of BOUND\n");
				break;
			}
			else if (cub->u_map.map[mapY][mapX] == '1')
				hit = 1;
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
		if (drawEnd >= WIN_HEIGHT)
			drawEnd = WIN_HEIGHT - 1;
		int color = 0x00FF00;
		if (mapX < 0 || mapX >= (int)ft_strlen(cub->u_map.map[mapY])
        		|| mapY < 0 ||  mapY >= height)
    		color = 0xFFFFFF; // default color for out-of-bounds
		else if (cub->u_map.map[mapY][mapX] == '1')
		{
    		color = 0xFF0000; // color for walls
		}
		else
		{
    		color = 0x00FF00; // color for empty space
		}
		if (side == 1)
			color = color / 2;
		verLine(cub, x, drawStart, drawEnd, color);
		x++;
	}
}

int render(t_main *cub)
{
	render_background(cub);
	cast_rays(cub);
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
	init_player(cub->player, cub, cub->u_map);
	mlx_loop_hook(cub->mlx_ptr, &render, cub);
	mlx_hook(cub->win_ptr, 2, 1L << 0, key_handle, cub);
	//mlx_hook(cub->win_ptr, DestroyNotify, KeyReleaseMask, mlx_loop_end, cub->mlx_ptr);
	mlx_hook(cub->win_ptr, 17, 0L, close_window, cub);
	mlx_loop(cub->mlx_ptr);
	return (SUCCESS);
}
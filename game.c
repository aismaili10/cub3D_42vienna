/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/01 22:14:57 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/* int	close_window(t_main *cub)
{
	write(1, COLOR_YELLOW"Closing Window\n"COLOR_RESET, 27);
	cleanup(cub, 3);
	return (SUCCESS);
} */

/* int	key_handle(int keycode, t_main *cub)
{
	if (keycode == ESC)
	{
		write(1, COLOR_YELLOW"Closing Window\n"COLOR_RESET, 27);
		cleanup(cub, 3);
	}
	return (SUCCESS);
} */

int key_states[256] = {0}; // Array to hold the state of keys


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
					player->planeY = 0.66;
				}
				if (u_map.map[y][x] == 'E')
				{
					player->dirX = 1;
					player->dirY = 0;
					player->planeX = 0;
					player->planeY = -0.66;
				}
				if (u_map.map[y][x] == 'S') // north and south might be mixed up
				{
					player->dirX = 0;
					player->dirY = 1;
					player->planeX = 0.66;
					player->planeY = 0;
				}
				if (u_map.map[y][x] == 'N')
				{
					player->dirX = 0;
					player->dirY = -1;
					player->planeX = -0.66;
					player->planeY = 0;
				}
				printf("Player initialized at position: (%f, %f)\n", player->posX, player->posY);
                return;
			}
		}
	}
}

void	move_player(t_main *cub, double moveX, double moveY)
{
	int	newPosX;
	int	newPosY;
	char	mapCell;
	static int i = 0;

	newPosX = (int)(cub->player->posX + moveX);
	newPosY = (int)(cub->player->posY + moveY);

	if (newPosX < 0 || newPosX >= cub->u_map.width
		|| newPosY < 0 || newPosY >= cub->u_map.width)
		{
			printf("Out of bounds\n");
			return ;
		}
	mapCell = cub->u_map.map[newPosY][newPosX];

	if (mapCell == '0' || mapCell == 'N' || mapCell == 'S' || mapCell == 'W' ||  mapCell == 'E') //here we also need check for N,S,E,W
	{
		cub->player->posX += moveX;
		cub->player->posY += moveY;
	}
	else
	{
        i++;
		//printf("moveX: %f and moveY: %f\n",moveX, moveY);
		printf("Collision detected with %c at map[%i][%i], movement blocked %d\n", mapCell, newPosY, newPosX, i); // Debug print
		//print_map_elements(&cub->u_map);
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
					pixel_put(&cub->mlx_img, x, y, 0x0EA5C0); // cub->u_map.f_color
				else
					pixel_put(&cub->mlx_img, x, y, 0x0F7910); // cub->u_map.f_color
				if (x < 5 || x > WIN_WIDTH - 5 || y < 5 || y > WIN_HEIGHT - 5) // not really necessary here, just blackened the edges
					pixel_put(&cub->mlx_img, x, y, 0x000000);
				x++;
			}
			y++;
		}
	}
	return (SUCCESS);
}
	//mlx_put_image_to_window(cub->mlx_ptr, cub->win_ptr, cub->mlx_img.img_ptr, 0, 0);

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
				printf("OUT of BOUND with x: %i >= len: %i; y: %i >= height: %i\n", mapX, cub->u_map.width, mapY, height);
				break;
			}
			else if (cub->u_map.map[mapY][mapX] == '1')
			{
				printf("cub->u_map.map[%i][%i]: %c\n", mapY, mapX, cub->u_map.map[mapY][mapX]);
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
		int color = 0x00FF00;
		if (mapX < 0 || mapX >= width
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
		printf("drawStart: %i and drawEnd: %i\n", drawStart, drawEnd);
		verLine(cub, x, drawStart, drawEnd, color);
		x++;
	}
}

int key_down(int keycode, t_main *cub) {
	if (keycode == KEY_LEFT)
		cub->key_states.left = 1;
	if (keycode == KEY_RIGHT)
		cub->key_states.right = 1;
	if (keycode == KEY_W)
		cub->key_states.w = 1;
	if (keycode == KEY_S)
		cub->key_states.s = 1;
	if (keycode == KEY_A)
		cub->key_states.a = 1;
	if (keycode == KEY_D)
		cub->key_states.d = 1;
	if (keycode == ESC)
		cub->key_states.esc = 1;
	return 0;
}

int key_up(int keycode, t_main *cub) {
	if (keycode == KEY_LEFT)
		cub->key_states.left = 0;
	if (keycode == KEY_RIGHT)
		cub->key_states.right = 0;
	if (keycode == KEY_W)
		cub->key_states.w = 0;
	if (keycode == KEY_S)
		cub->key_states.s = 0;
	if (keycode == KEY_A)
		cub->key_states.a = 0;
	if (keycode == KEY_D)
		cub->key_states.d = 0;
	return 0;
}

void process_input(t_main *cub)
{
	if (cub->key_states.esc)
		cleanup(cub, 3);
	if (cub->key_states.left)
		rotate_player(cub->player, -ROT_SPEED);
	if (cub->key_states.right)
		rotate_player(cub->player, ROT_SPEED);
	if (cub->key_states.w)
		move_player(cub, cub->player->dirX * MOVE_SPEED, cub->player->dirY * MOVE_SPEED);
	if (cub->key_states.s)
		move_player(cub, -cub->player->dirX * MOVE_SPEED, -cub->player->dirY * MOVE_SPEED);
	if (cub->key_states.a)
		move_player(cub, -cub->player->dirY * MOVE_SPEED, cub->player->dirX * MOVE_SPEED);
	if (cub->key_states.d)
		move_player(cub, cub->player->dirY * MOVE_SPEED, -cub->player->dirX * MOVE_SPEED);
}

/* #define MINIMAP_WIDTH 50  // Width of minimap in cells
#define MINIMAP_HEIGHT 50 // Height of minimap in cells
#define PLAYER_RADIUS 3 // Radius of the player on the minimap

void draw_square(t_main *cub, int x, int y, int color) {
    for (int i = 0; i < MINIMAP_SCALE; i++) {
        for (int j = 0; j < MINIMAP_SCALE; j++) {
            pixel_put(&cub->mlx_img, x + i, y + j, color);
        }
    }
}

void draw_circle(t_main *cub, int cx, int cy, int radius, int color) {
    int x, y;

    for (y = -radius; y <= radius; y++) {
        for (x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                pixel_put(&cub->mlx_img, cx + x, cy + y, color);
            }
        }
    }
} */

//#define MINIMAP_SCALE 5   // Scale for minimap size

/* void draw_minimap(t_main *cub) {
    int startX, startY, endX, endY;
    int playerX, playerY;

    // Calculate viewport boundaries
    playerX = (int)cub->player->posX;
    playerY = (int)cub->player->posY;
    startX = playerX - MINIMAP_WIDTH / 2;
    startY = playerY - MINIMAP_HEIGHT / 2;
    endX = playerX + MINIMAP_WIDTH / 2;
    endY = playerY + MINIMAP_HEIGHT / 2;

    // Clamp boundaries to map limits
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX >= cub->u_map.width) endX = cub->u_map.width - 1;
    if (endY >= cub->u_map.height) endY = cub->u_map.height - 1;

    // Draw the minimap grid
    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            int color = (cub->u_map.map[y][x] == '1') ? 0x888888 : 0xFFFFFF;
            draw_square(cub, (x - startX) * MINIMAP_SCALE, (y - startY) * MINIMAP_SCALE, color);
        }
    }

    // Draw the player as a circle
    int px = (cub->player->posX - startX) * MINIMAP_SCALE;
    int py = (cub->player->posY - startY) * MINIMAP_SCALE;
    draw_circle(cub, px, py, PLAYER_RADIUS, 0xFF0000);
} */


int render(t_main *cub)
{
	process_input(cub);  // Process input before rendering
	render_background(cub);
	cast_rays(cub);
	//draw_minimap(cub); // Call minimap rendering
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

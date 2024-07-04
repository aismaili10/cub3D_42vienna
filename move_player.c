/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:45:44 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/04 13:48:17 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
		//printf("Collision detected with %c at map[%i][%i], movement blocked %d\n", mapCell, newPosY, newPosX, i); // Debug print
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

int key_down(int keycode, t_main *cub) 
{
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

int key_up(int keycode, t_main *cub) 
{
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
		rotate_player(cub->player, ROT_SPEED);
	if (cub->key_states.right)
		rotate_player(cub->player, -ROT_SPEED);
	if (cub->key_states.w)
		move_player(cub, cub->player->dirX * MOVE_SPEED, cub->player->dirY * MOVE_SPEED);
	if (cub->key_states.s)
		move_player(cub, -cub->player->dirX * MOVE_SPEED, -cub->player->dirY * MOVE_SPEED);
	if (cub->key_states.a)
		move_player(cub, cub->player->dirY * MOVE_SPEED, -cub->player->dirX * MOVE_SPEED);
	if (cub->key_states.d)
		move_player(cub, -cub->player->dirY * MOVE_SPEED, cub->player->dirX * MOVE_SPEED);
}
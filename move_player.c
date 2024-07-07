/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:45:44 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/07 11:07:51 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	move_player(t_main *cub, double move_x, double move_y)
{
	int		newpos_x;
	int		newpos_y;
	char	map_cell;
	double	factor;

	factor = 10.0;
	newpos_x = (int)(cub->player->pos_x + move_x * factor);
	newpos_y = (int)(cub->player->pos_y + move_y * factor);
	if (newpos_x < 0 || newpos_x >= cub->u_map.width
		|| newpos_y < 0 || newpos_y >= cub->u_map.height)
		return ;
	map_cell = cub->u_map.map[newpos_y][newpos_x];
	if (ft_strchr("0NSWE", map_cell)
		&& check_edge(&cub->u_map, cub->player, newpos_x, newpos_y))
	{
		cub->player->pos_x += move_x;
		cub->player->pos_y += move_y;
	}
}

void	rotate_player(t_player *player, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = player->dir_x;
	old_plane_x = player->plane_x;
	player->dir_x = player->dir_x * cos(angle) - player->dir_y * sin(angle);
	player->dir_y = old_dir_x * sin(angle) + player->dir_y * cos(angle);
	player->plane_x = player->plane_x * cos(angle)
		- player->plane_y * sin(angle);
	player->plane_y = old_plane_x * sin(angle) + player->plane_y * cos(angle);
}

int	key_down(int keycode, t_main *cub)
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
	if (keycode == KEY_ESC)
		cub->key_states.esc = 1;
	return (0);
}

int	key_up(int keycode, t_main *cub)
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
	return (0);
}

void	process_input(t_main *cub)
{
	if (cub->key_states.esc)
		cleanup(cub, 3);
	if (cub->key_states.left)
		rotate_player(cub->player, ROT_SPEED);
	if (cub->key_states.right)
		rotate_player(cub->player, -ROT_SPEED);
	if (cub->key_states.w)
		move_player(cub, cub->player->dir_x * MOVE_SPEED,
			cub->player->dir_y * MOVE_SPEED);
	if (cub->key_states.s)
		move_player(cub, -cub->player->dir_x * MOVE_SPEED,
			-cub->player->dir_y * MOVE_SPEED);
	if (cub->key_states.a)
		move_player(cub, cub->player->dir_y * MOVE_SPEED,
			-cub->player->dir_x * MOVE_SPEED);
	if (cub->key_states.d)
		move_player(cub, -cub->player->dir_y * MOVE_SPEED,
			cub->player->dir_x * MOVE_SPEED);
}

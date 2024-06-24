/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:23:11 by aszabo            #+#    #+#             */
/*   Updated: 2024/06/22 13:37:48 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	close_window(t_main *cub)
{
	write(1, COLOR_YELLOW"Closing Window\n"COLOR_RESET, 27);
	cleanup(cub, 2);
	return (SUCCESS);
}

int	key_handle(int keycode, t_main *cub)
{
	if (keycode == ESC)
	{
		write(1, COLOR_YELLOW"Closing Window\n"COLOR_RESET, 27);
		cleanup(cub, 2);
	}
	return (SUCCESS);
}

int	init_mlx(t_main *cub)
{
	cub->mlx_ptr = mlx_init();
	if (!cub->mlx_ptr)
		return (cleanup(cub, 2), SYS_FAIL);
	cub->win_ptr = mlx_new_window(cub->mlx_ptr, 1000, 800, "cub3D"); //temporary width and height
	if (!cub->win_ptr)
	{
		free(cub->mlx_ptr);
		return (cleanup(cub, 2), SYS_FAIL);
	}
	cub->mlx_img.img_ptr = mlx_new_image(cub->mlx_ptr, 1000, 800); //temporary width and height
	if (!cub->mlx_img.img_ptr)
	{
		mlx_destroy_window(cub->mlx_ptr, cub->win_ptr);
		free(cub->mlx_ptr);
		free(cub->win_ptr);
		return (cleanup(cub, 2), SYS_FAIL);
	}
	cub->mlx_img.addr = mlx_get_data_addr(cub->mlx_img.img_ptr, 
			&cub->mlx_img.bpp, &cub->mlx_img.line_length, &cub->mlx_img.endian);
	if (!cub->mlx_img.addr)
	{
		mlx_destroy_image(cub->mlx_ptr, cub->mlx_img.img_ptr);
		mlx_destroy_window(cub->mlx_ptr, cub->win_ptr);
		free(cub->mlx_img.img_ptr);
		free(cub->mlx_ptr);
		free(cub->win_ptr);
		return (cleanup(cub, 2), SYS_FAIL);
	}
	mlx_hook(cub->win_ptr, 2, 1, key_handle, cub);
	mlx_hook(cub->win_ptr, 17, 0, close_window, cub);
	mlx_loop(cub->mlx_ptr);
	return (SUCCESS);
}

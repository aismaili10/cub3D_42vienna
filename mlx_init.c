/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:23:11 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 13:01:18 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_mlx(t_main *cub)
{
	cub->mlx_ptr = mlx_init();
	if (!cub->mlx_ptr)
		return (cleanup(cub, 2), SYS_FAIL);
	cub->win_ptr = mlx_new_window(cub->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "cub3D");
	if (!cub->win_ptr)
		return (free(cub->mlx_ptr), cleanup(cub, 2), SYS_FAIL);
	cub->mlx_img.img_ptr = mlx_new_image(cub->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!cub->mlx_img.img_ptr)
	{
		mlx_destroy_window(cub->mlx_ptr, cub->win_ptr);
		free(cub->mlx_ptr);
		return (cleanup(cub, 2), SYS_FAIL);
	}
	cub->mlx_img.addr = mlx_get_data_addr(cub->mlx_img.img_ptr,
			&cub->mlx_img.bpp, &cub->mlx_img.line_length, &cub->mlx_img.endian);
	if (!cub->mlx_img.addr)
	{
		mlx_destroy_image(cub->mlx_ptr, cub->mlx_img.img_ptr);
		mlx_destroy_window(cub->mlx_ptr, cub->win_ptr);
		free(cub->mlx_ptr);
		return (cleanup(cub, 2), SYS_FAIL);
	}
	return (SUCCESS);
}

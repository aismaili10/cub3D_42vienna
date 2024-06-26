/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/06/26 08:51:27 by aszabo           ###   ########.fr       */
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

void	pixel_put(t_img *mlx_img, int x, int y, int color)
{
	char	*dst;

	dst = mlx_img->addr + (y * mlx_img->line_length + x * (mlx_img->bpp / 8));
	*(unsigned int*)dst = color;
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
					pixel_put(&cub->mlx_img, x, y, cub->u_map.f_color);
				else
					pixel_put(&cub->mlx_img, x, y, cub->u_map.c_color);
				if (x < 5 || x > WIN_WIDTH - 5 || y < 5 || y > WIN_HEIGHT - 5) // not really necessary here, just blackened the edges
					pixel_put(&cub->mlx_img, x, y, 0x000000);
				x++;
			}
			y++;
		}
	}
	mlx_put_image_to_window(cub->mlx_ptr, cub->win_ptr, cub->mlx_img.img_ptr, 0, 0);
	return (SUCCESS);
}

int render(t_main *cub)
{
	render_background(cub);
	return (SUCCESS);
}

int game(t_main *cub)
{
	mlx_loop_hook(cub->mlx_ptr, &render, cub);
	//mlx_hook(cub->win_ptr, 2, 1L << 0, key_handle, cub);
	mlx_hook(cub->win_ptr, DestroyNotify, KeyReleaseMask, mlx_loop_end, cub->mlx_ptr);
	mlx_loop(cub->mlx_ptr);
	return (SUCCESS);
}
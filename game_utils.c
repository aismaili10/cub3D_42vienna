/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:52:47 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/06 20:14:59 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	close_window(t_main *cub)
{
	cleanup(cub, 3);
	return (0);
}

void	pixel_put(t_img *mlx_img, int x, int y, int color)
{
	char	*dst;

	dst = mlx_img->addr + (y * mlx_img->line_length + x * (mlx_img->bpp / 8));
	*(unsigned int *)dst = color;
}

int	render_background(t_main *cub)
{
	int	x;
	int	y;

	y = 0;
	if (cub->mlx_ptr)
	{
		while (y < WIN_HEIGHT)
		{
			x = 0;
			while (x < WIN_WIDTH)
			{
				if (y < WIN_HEIGHT / 2)
					pixel_put(&cub->mlx_img, x, y, cub->u_map.c_color);
				else
					pixel_put(&cub->mlx_img, x, y, cub->u_map.f_color);
				x++;
			}
			y++;
		}
	}
	return (SUCCESS);
}

int	check_texture_size(t_main *cub)
{
	if (cub->north->width < TEX_WIDTH || cub->north->height < TEX_HEIGHT
		|| cub->north->width > TEX_WIDTH || cub->north->height > TEX_HEIGHT)
		return (FAILURE);
	if (cub->south->width < TEX_WIDTH || cub->south->height < TEX_HEIGHT
		|| cub->south->width > TEX_WIDTH || cub->south->height > TEX_HEIGHT)
		return (FAILURE);
	if (cub->east->width < TEX_WIDTH || cub->east->height < TEX_HEIGHT
		|| cub->east->width > TEX_WIDTH || cub->east->height > TEX_HEIGHT)
		return (FAILURE);
	if (cub->west->width < TEX_WIDTH || cub->west->height < TEX_HEIGHT
		|| cub->west->width > TEX_WIDTH || cub->west->height > TEX_HEIGHT)
		return (FAILURE);
	return (SUCCESS);
}

int	handle_textures(t_main *cub)
{
	if (init_textures(cub) == FAILURE)
		return (FAILURE);
	if (load_textures(cub) == FAILURE)
	{
		write(2, RD "Error\nInvalid texture path\n" CR, 40);
		return (free_texture_pointers(cub), FAILURE);
	}
	if (check_texture_size(cub) == FAILURE)
	{
		write(2, RD "Error\nInvalid texture size\n" CR, 40);
		free_texture_mlx(cub);
		return (free_texture_pointers(cub), FAILURE);
	}
	if (create_texture_buffer(cub) == FAILURE)
	{
		free_texture_mlx(cub);
		return (free_texture_pointers(cub), FAILURE);
	}
	return (SUCCESS);
}

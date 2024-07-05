/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:08:32 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 15:05:52 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	load_textures_2(t_main *cub)
{
	cub->west->img_ptr = mlx_xpm_file_to_image(cub->mlx_ptr, cub->u_map.we,
			&cub->west->width, &cub->west->height);
	if (!cub->west->img_ptr)
	{
		mlx_destroy_image(cub->mlx_ptr, cub->south->img_ptr);
		return (mlx_destroy_image(cub->mlx_ptr, cub->north->img_ptr), FAILURE);
	}
	cub->west->addr = mlx_get_data_addr(cub->west->img_ptr, &cub->west->bpp,
			&cub->west->line_length, &cub->west->endian);
	cub->east->img_ptr = mlx_xpm_file_to_image(cub->mlx_ptr, cub->u_map.ea,
			&cub->east->width, &cub->east->height);
	if (!cub->east->img_ptr)
	{
		mlx_destroy_image(cub->mlx_ptr, cub->south->img_ptr);
		mlx_destroy_image(cub->mlx_ptr, cub->west->img_ptr);
		return (mlx_destroy_image(cub->mlx_ptr, cub->north->img_ptr), FAILURE);
	}
	cub->east->addr = mlx_get_data_addr(cub->east->img_ptr, &cub->east->bpp,
			&cub->east->line_length, &cub->east->endian);
	return (SUCCESS);
}

int	load_textures(t_main *cub)
{
	cub->north->img_ptr = mlx_xpm_file_to_image(cub->mlx_ptr, cub->u_map.no,
			&cub->north->width, &cub->north->height);
	if (!cub->north->img_ptr)
		return (FAILURE);
	cub->north->addr = mlx_get_data_addr(cub->north->img_ptr, &cub->north->bpp,
			&cub->north->line_length, &cub->north->endian);
	cub->south->img_ptr = mlx_xpm_file_to_image(cub->mlx_ptr, cub->u_map.so,
			&cub->south->width, &cub->south->height);
	if (!cub->south->img_ptr)
		return (mlx_destroy_image(cub->mlx_ptr, cub->north->img_ptr), FAILURE);
	cub->south->addr = mlx_get_data_addr(cub->south->img_ptr, &cub->south->bpp,
			&cub->south->line_length, &cub->south->endian);
	if (load_textures_2(cub) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	init_textures(t_main *cub)
{
	t_texture	*west;
	t_texture	*east;
	t_texture	*north;
	t_texture	*south;

	west = malloc(sizeof(t_texture) * 1);
	if (!west)
		return (FAILURE);
	east = malloc(sizeof(t_texture) * 1);
	if (!east)
		return (free(west), FAILURE);
	south = malloc(sizeof(t_texture) * 1);
	if (!south)
		return (free(west), free(east), FAILURE);
	north = malloc(sizeof(t_texture) * 1);
	if (!north)
		return (free(west), free(east), free(south), FAILURE);
	cub->west = west;
	cub->east = east;
	cub->north = north;
	cub->south = south;
	return (SUCCESS);
}

int	*fill_buffer(t_texture *texture)
{
	int	*texture_buff;
	int	i;
	int	j;

	texture_buff = malloc(sizeof(int) * texture->width * texture->height);
	if (!texture_buff)
		return (NULL);
	i = 0;
	while (i < texture->height)
	{
		j = 0;
		while (j < texture->width)
		{
			texture_buff[i * texture->width + j] = *(int *)(texture->addr
					+ (i * texture->line_length + j * (texture->bpp / 8)));
			j++;
		}
		i++;
	}
	return (texture_buff);
}

int	create_texture_buffer(t_main *cub)
{
	int	*texture_buff[4];

	texture_buff[0] = fill_buffer(cub->north);
	if (!texture_buff[0])
		return (FAILURE);
	texture_buff[1] = fill_buffer(cub->south);
	if (!texture_buff[1])
		return (free(texture_buff[0]), FAILURE);
	texture_buff[2] = fill_buffer(cub->west);
	if (!texture_buff[2])
		return (free(texture_buff[0]), free(texture_buff[1]), FAILURE);
	texture_buff[3] = fill_buffer(cub->east);
	if (!texture_buff[3])
		return (free(texture_buff[0]), free(texture_buff[1]),
			free(texture_buff[2]), FAILURE);
	cub->texture_buff[0] = texture_buff[0];
	cub->texture_buff[1] = texture_buff[1];
	cub->texture_buff[2] = texture_buff[2];
	cub->texture_buff[3] = texture_buff[3];
	return (SUCCESS);
}

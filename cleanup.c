/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:27 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/06 15:38:17 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	clean_mlx(t_main *cub)
{
	mlx_destroy_image(cub->mlx_ptr, cub->mlx_img.img_ptr);
	mlx_destroy_window(cub->mlx_ptr, cub->win_ptr);
	mlx_destroy_display(cub->mlx_ptr);
	free(cub->mlx_ptr);
}

void	free_texture_pointers(t_main *cub)
{
	free(cub->north);
	free(cub->south);
	free(cub->west);
	free(cub->east);
}

void	free_texture_mlx(t_main *cub)
{
	mlx_destroy_image(cub->mlx_ptr, cub->north->img_ptr);
	mlx_destroy_image(cub->mlx_ptr, cub->south->img_ptr);
	mlx_destroy_image(cub->mlx_ptr, cub->west->img_ptr);
	mlx_destroy_image(cub->mlx_ptr, cub->east->img_ptr);
}

void	free_texture_buff(t_main *cub)
{
	free(cub->texture_buff[0]);
	free(cub->texture_buff[1]);
	free(cub->texture_buff[2]);
	free(cub->texture_buff[3]);
}

void	cleanup(t_main *cub, int stage)
{
	if (stage == -1)
	{
		get_next_line(cub->u_map.fd, 1);
		free_map_elements(cub);
	}
	if (stage == 2)
		free_map_elements(cub);
	if (stage == 3)
	{
		free_texture_buff(cub);
		free_texture_mlx(cub);
		free_texture_pointers(cub);
		free_map_elements(cub);
		clean_mlx(cub);
		free(cub->render);
		free(cub->player);
	}
	if (stage == 4)
	{
		free_map_elements(cub);
		clean_mlx(cub);
	}
	exit(stage);
}

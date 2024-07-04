/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:27 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/04 18:12:50 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_textures(t_map *u_map)
{
	if (u_map->no)
		free(u_map->no);
	if (u_map->so)
		free(u_map->so);
	if (u_map->we)
		free(u_map->we);
	if (u_map->ea)
		free(u_map->ea);
}

void	free_map(char **map)
{
	int	i;

	i = -1;
	if (!map)
		return ;
	while (map[++i])
		free(map[i]);
	free(map);
}

void	free_map_elements(t_main *cub)
{
	if (cub->u_map.fd != -1)
		close(cub->u_map.fd);
	free_textures(&cub->u_map);
	if (cub->u_map.joined_lines)
		free(cub->u_map.joined_lines);
	free_str_array(&cub->u_map.spl_ln);
	free_map(cub->u_map.map);
}

void	clean_mlx(t_main *cub)
{
		mlx_destroy_image(cub->mlx_ptr, cub->mlx_img.img_ptr);
		mlx_destroy_window(cub->mlx_ptr, cub->win_ptr);
		mlx_destroy_display(cub->mlx_ptr);
		/* free(cub->mlx_img.img_ptr);
		free(cub->win_ptr); */
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
	(void)cub;
	(void)stage;
	printf("arrived in cleanup with stage: %i\n", stage);
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
		free(cub->player);
	}
	if (stage == 10)
		printf(COLOR_GREEN"map is valid\n"COLOR_RESET);
	exit(stage);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 08:22:53 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 12:30:56 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_loop(t_main *cub, t_player *player, t_render *render, int x)
{
	int	tex_x;
	int	tex_y;
	int	color;
	int	d;
	int	y;

	tex_x = (int)(render->wallX * (double)TEX_WIDTH);
	if (render->side == 0 && player->rayDirX > 0)
		tex_x = TEX_WIDTH - tex_x - 1;
	if (render->side == 1 && player->rayDirY < 0)
		tex_x = TEX_WIDTH - tex_x - 1;
	y = render->drawStart;
	while (y < render->drawEnd)
	{
		d = y * 256 - WIN_HEIGHT * 128 + render->lineHeight * 128;
		tex_y = ((d * TEX_HEIGHT) / render->lineHeight) / 256;
		color = cub->texture_buff[render->texIndex][TEX_HEIGHT * tex_y + tex_x];
		pixel_put(&cub->mlx_img, x, y, color);
		y++;
	}
}

void	cast_rays(t_main *cub)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		get_ray_direction(cub->player, x);
		get_distances(cub->player, cub->render);
		hit_wall_loop(cub, cub->render);
		get_draw_values(cub->player, cub->render);
		get_texture_index(cub->player, cub->render);
		draw_loop(cub, cub->player, cub->render, x);
		x++;
	}
}

int	render(t_main *cub)
{
	process_input(cub);
	render_background(cub);
	cast_rays(cub);
	draw_minimap(cub);
	mlx_put_image_to_window(cub->mlx_ptr,
		cub->win_ptr, cub->mlx_img.img_ptr, 0, 0);
	return (SUCCESS);
}

int	game(t_main *cub)
{
	t_player	*player;
	t_render	*render_vals;

	player = malloc(sizeof(t_player));
	if (!player)
		return (FAILURE);
	render_vals = malloc(sizeof(t_render));
	if (!render_vals)
		return (free(player), FAILURE);
	cub->player = player;
	cub->render = render_vals;
	cub->u_map.height = str_ary_len(cub->u_map.map);
	cub->u_map.width = ft_strlen(cub->u_map.map[0]);
	cub->key_states = (t_key_states){0}; // Initialize key states
	if (handle_textures(cub) == FAILURE)
		return (FAILURE);
	init_player(cub->player, cub->u_map);
	mlx_hook(cub->win_ptr, 2, 1L << 0, key_down, cub);// Key press
    mlx_hook(cub->win_ptr, 3, 1L << 1, key_up, cub);// Key release
	mlx_loop_hook(cub->mlx_ptr, &render, cub);
	// mlx_hook(cub->win_ptr, 2, 1L << 0, key_handle, cub);
	//mlx_hook(cub->win_ptr, DestroyNotify, KeyReleaseMask, mlx_loop_end, cub->mlx_ptr);
	mlx_hook(cub->win_ptr, 17, 0L, close_window, cub);
	mlx_loop(cub->mlx_ptr);
	return (SUCCESS);
}

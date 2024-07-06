/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:48:01 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/06 14:53:11 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	assign_default(t_main *cub)
{
	cub->u_map.fd = -1;
	cub->u_map.c_color = -1;
	cub->u_map.f_color = -1;
	cub->u_map.map = NULL;
	cub->u_map.no = NULL;
	cub->u_map.so = NULL;
	cub->u_map.we = NULL;
	cub->u_map.ea = NULL;
	cub->u_map.spl_ln = NULL;
	cub->u_map.joined_lines = NULL;
	cub->u_map.id_ed = false;
}

int	main(int ac, char *av[])
{
	t_main	cub;

	if (ac != 2)
		return (write(2, CY"Usage:\n./cub3D path_to_map_file\n"CR, 44), 1);
	assign_default(&cub);
	if (map_val(&cub, av[1]) != SUCCESS)
		return (write(2, RD"Invalid Map\n"CR, 24), 2);
	if (init_mlx(&cub) != SUCCESS)
	{
		write(2, CG"MLX Initialization Failed\n"CR, 32);
		cleanup(&cub, 2);
	}
	if (game(&cub) == FAILURE)
	{
		cleanup(&cub, 4);
		return (write(2, RD"Game Failed\n"CR, 20), 4);
	}
	return (SUCCESS);
}

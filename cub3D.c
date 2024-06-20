/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:48:01 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/18 19:14:08 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	assign_default(t_main *cub)
{
	// u_map
	cub->u_map.fd = -1;
	cub->u_map.c_color = -1;
	cub->u_map.f_color = -1;
	cub->u_map.map = NULL;
	cub->u_map.no = NULL;
	cub->u_map.so = NULL;
	cub->u_map.we = NULL;
	cub->u_map.ea = NULL;
	cub->u_map.splited_line = NULL;
	cub->u_map.joined_lines = NULL;
	cub->u_map.id_ed = false;
	// other cub members; unknown now
	// ...
}

int	main(int ac, char *av[])
{
	t_main	cub; // info: no allocation needed for cubed, because declared in main
	if (ac != 2)
		return (write(2, COLOR_YELLOW"Usage:\n./cub3D path_to_map_file\n"COLOR_RESET, 44), 1); // ?!maybe write into stderr!?
	assign_default(&cub);
	if (map_val(&cub, av[1]) == -1)
		return (write(2, COLOR_RED"Invalid Map\n"COLOR_RESET, 24), 2);
	// if (init_mlx(&cub) == -1)
	// 	return (write(2, COLOR_GREEN"MLX Initialization Failed\n"COLOR_RESET, 32), 3);
	printf(COLOR_GREEN"Map %s is Valid\n"COLOR_RESET, av[1]);
	cleanup(&cub, 10);
	return (SUCCESS);
}

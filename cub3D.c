/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:48:01 by aismaili          #+#    #+#             */
/*   Updated: 2024/05/31 17:10:15 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	assign_default(t_main *cub)
{
	// u_map
	cub->u_map.fd = -1;
	cub->u_map.c_color = -1;
	cub->u_map.f_color = -1;
	cub->u_map.no = NULL;
	cub->u_map.so = NULL;
	cub->u_map.we = NULL;
	cub->u_map.ea = NULL;
	cub->u_map.splited_line = NULL;
	// other cub members; unknown now
	// ...
}

int	main(int ac, char *av[])
{
	t_main	cub; // info: no allocation needed for cubed, because declared in main
	if (ac != 2)
		return (write(2, "Usage:\n./cub3D path_to_map_file\n", 33), 1); // ?!maybe write into stderr!?
	assign_default(&cub);
	// are there checks for the map file ending?
	if (map_val(&cub, av[1]) == -1) // maybe 'exit' already inside the functions, instead of returning in the main!?
		return (write(2, "invalid map\n", 13), 2);
	// read / get_next_line() to fill a string array / map

	return (SUCCESS);
}

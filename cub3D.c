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
	cub->u_map.id_ed = false;
	// other cub members; unknown now
	// ...
}

void	print_map_elements(t_map* u_map)
{
	printf("MAP ELEMENTS\n");
	printf("NO: %s\n", u_map->no);
	printf("SO: %s\n", u_map->so);
	printf("WE: %s\n", u_map->we);
	printf("EA: %s\n", u_map->ea);
	printf("Floor Color: %i\n", u_map->f_color);
	printf("Ceiling Color: %i\n", u_map->c_color);
}

int	main(int ac, char *av[])
{
	t_main	cub; // info: no allocation needed for cubed, because declared in main
	if (ac != 2)
		return (write(2, COLOR_YELLOW"Usage:\n./cub3D path_to_map_file\n"COLOR_RESET, 44), 1); // ?!maybe write into stderr!?
	assign_default(&cub);
	if (map_val(&cub, av[1]) == -1) // maybe 'exit' already inside the functions, instead of returning in the main!?
		return (write(2, COLOR_GREEN"invalid map\n"COLOR_GREEN, 27), 2);

	//print_map_elements(&cub.u_map);
	//printf("map %s is valid\n", av[1]);
	printf(COLOR_GREEN"map %s is valid\n"COLOR_RESET, av[1]);
	return (SUCCESS);
}

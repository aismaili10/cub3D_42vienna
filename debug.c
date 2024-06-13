/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:55:08 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/13 15:17:35 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	print_map(char **map)
{
	int	i;

	i = -1;
	while (map[++i])
		printf("%s\n", map[i]);
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
	printf("THE MAP:\n");
	print_map(u_map->map);
}

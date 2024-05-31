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

int	init_u_map()
{
	
}


int	map_val(t_main *cubed, char *map_path)
{
	// file name/type checks??
	// path checks??
	// open
	int	fd;
	char *tmp;

	fd = open(map_path, O_RDONLY);
	if (fd == -1)
		return (perror("open"), -1);
	// read the 4 textures and 2 colors
	errno = 0;
	while (1)
	{
		tmp = get_next_line(fd, 0);
		if (!tmp && errno)
		{
			// start cleaning and exit process
		}
		if (is_color()) // color
		{
			init_u_map();
		}
		else if (is_texture())
		{
			init_u_map();
		}
		free(tmp);
	}
}

int	main(int ac, char *av[])
{
	t_main	cubed;
	if (ac != 2)
		return (printf("Usage:\n./cub3D *.cub"), 1);
	// are there checks for the map file ending?
	// open the map file
	if (map_val(&cubed, av[1]) == -1);
		return (2);
	// read / get_next_line() to fill a string array / map
	// close as soon as not needed anymore --> after get_next_line();
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:15 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/05 15:44:16 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	closed_walls(char **map)
{
	int	i;
	int	j;
	int	p_counter;

	p_counter = 0;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (ft_strchr("0NSWE", map[i][j])
				&& check_pos(map, i, j) != SUCCESS)
				return (INV_MAP);
			if (ft_strchr("NSWE", map[i][j]))
				p_counter++;
			j++;
		}
		i++;
	}
	if (p_counter != 1)
	{
		printf("Player count: %i\n", p_counter);
		return (write(2, RD "ERROR: PLAYER PLACEMENT\n" CR, 36), INV_MAP);
	}
	return (SUCCESS);
}

int	add_spaces(char *str, int max)
{
	int	len;

	len = ft_strlen(str);
	while (len < max)
	{
		str[len] = ' ';
		len++;
	}
	str[len] = 0;
	return (SUCCESS);
}

int	add_generic_spaces(char **map)
{
	char	*temp;
	int		r;
	int		max_len;

	r = 0;
	max_len = get_max_len(map);
	while (map[r])
	{
		temp = malloc(max_len + 1);
		if (!temp)
			return (perror("malloc"), SYS_FAIL);
		ft_strlcpy(temp, map[r], ft_strlen(map[r]) + 1);
		// printf("temp: %s\n", temp);
		// printf(" map: %s\n", map[r]);
		add_spaces(temp, max_len);
		free(map[r]);
		map[r] = temp;
		r++;
	}
	return (SUCCESS);
}

int	check_map_element(t_main *cub, char *lines)
{
	// printf("lines: %s\n", lines);
	// printf("n_lines: %s\n", lines);
	cub->u_map.map = ft_split(lines, '\n');
	if (!cub->u_map.map)
		return (perror("malloc"), FAILURE);
	// if we DON'T ACCEPT lines with only spaces in the actual map, we look for them here after split
	//print_map_elements(&cub->u_map);
	if (l_only_space(cub->u_map.map) != SUCCESS)
		return (FAILURE);
	add_generic_spaces(cub->u_map.map);
	if (closed_walls(cub->u_map.map) != SUCCESS)
	{
		write(2, COLOR_RED "Walls Not Closed!\n" COLOR_RESET, 30);
		return (INV_MAP);
	}
	return (SUCCESS);
}

int	map_val(t_main *cub, char *map_path)
{
	if (!is_cub_postfix(map_path))
		return (write(2, RD"Invalid File Postfix\n"CR, 33), INV_MAP);
	cub->u_map.fd = open(map_path, O_RDONLY);
	if (cub->u_map.fd == -1)
		return (perror("open"), -1); // nothing to free / clean at this stage
	// read the 4 textures and 2 colors
	if (read_check_txts_clrs(cub) != SUCCESS)
		cleanup(cub, 1);
	if (read_map_element(cub) != SUCCESS)
		cleanup(cub, 2);
	if (check_map_element(cub, cub->u_map.joined_lines) != SUCCESS)
		cleanup(cub, 2);
	if (check_texture_path(cub) != SUCCESS)
		cleanup(cub, 2);
	return (SUCCESS);
}

/*

// not needed because even valid maps, separated by nl are invalid
	lines = add_generic_space(lines);
	if (!lines)
		return (perror(malloc), FAILURE);

int ft_count_char(char *str, char c)
{
	int counter;

	counter = 0;
	if (!str)
		return (counter);
	while (*str)
	{
		if (*str == c)
			counter++;
		str++;
	}
	return (counter);
}

char	*add_generic_space(char *lines)
{
	char	*new_lines;
	char	*store_nl;
	char	*store_l;

	new_lines = malloc(ft_strlen(lines) + ft_count_char(lines, '\n') + 1);
	if (!new_lines)
		return (NULL);
	store_nl = new_lines;
	store_l = lines;
	while (*lines)
	{
		*new_lines = *lines;
		if (*lines == '\n')
			*++new_lines = ' ';
		++lines;
		++new_lines;
	}
	*new_lines = 0;
	return (free(store_l), store_nl);
}
*/

/*
bool only_space_so_empty(char *line) // only spaces
{
	while (*line)
	{
		if (*line == ' ')
			line++;
		else
			return (false);
	}
	return (true);
}

bool lower_wall(char **map, int i)
{
	if (only_space_so_empty(map[i]))
		return (false);
	else if (i < str_ary_len(map) && only_space_so_empty(map[i + 1]))
		return (true);
	return (true);
}

bool upper_wall(char **map, int i)
{
	if (only_space_so_empty(map[i]))
		return (false);
	else if (i > 0 && only_space_so_empty(map[i - 1]))
		return (true);
	return (true);
}*/

/* int	gap_in_wall(char **map, int l)
{
	int	c;

	c = 0;
	// take each 0 and the Player and see whether it is within walls
	while (map[l][c])
	{

	}
} */

/*

1 1 1 1

 1111111
100000001
10N000001
100000001
11111111

   1 1 1

*/

// int	outside_checks(char **map, int i)
// {
// 	if (/* upperwall(map, i) &&  */gap_in_wall(map, i) == INV_MAP)
// 		return (INV_MAP);
// 	if (/* lower_wall(map, i) &&  */gap_in_wall(map, i) == INV_MAP)
// 		return (INV_MAP);
// 	check_sides();
// 	return (SUCCESS);
// }

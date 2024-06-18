/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:15 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/18 22:37:37 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	invalid_element(char *line)
{
	if (is_whitespace(line[0]) || is_whitespace(line[ft_strlen(line) - 1]))
	{
		write(2, COLOR_RED"Whitespace at Line-Beginning/End\n"COLOR_RESET, 48);
		return (INV_MAP);
	}
	return (SUCCESS);
}

int	prep_for_init(char *line, t_main *cub)
{
	errno = 0;
	if (invalid_element(line) == INV_MAP)
		return (free(line), INV_MAP);
	cub->u_map.splited_line = ft_split_md(line, " \t");
	if (!cub->u_map.splited_line && errno)
		return (free(line), perror("malloc"), SYS_FAIL);
	if (str_ary_len(cub->u_map.splited_line) != 2 && ft_strncmp(line, "\n", 2))
	{
		write(2, COLOR_YELLOW"WARNING: Map Element Syntax: ", 37);
		write(2, "IDENTIFIER SPECIFIER\n"COLOR_RESET, 26);
		return (free(line), INV_MAP);
	}
	free(line);
	return (SUCCESS);
}

int	read_check_txts_clrs(t_main *cub)
{
	char	*tmp;

	errno = 0;
	while (!txts_clrs_found(&cub->u_map))
	{
		cub->u_map.id_ed = false;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
			cleanup(cub, 1);
		if (!tmp)
		{
			write(2, COLOR_RED"Incomplete Map File\n"COLOR_RESET, 32);
			cleanup(cub, 0);
		}
		if (prep_for_init(tmp, cub) != SUCCESS)
			cleanup(cub, 1);
		if (handle_color(tmp, cub) != SUCCESS || handle_texture(tmp, cub) != SUCCESS)
			cleanup(cub, 1);
		/*if (handle_texture(tmp, cub) != SUCCESS)
			cleanup(cub, 1);*/
		if (!cub->u_map.id_ed && ft_strncmp("\n", cub->u_map.splited_line[0], 2)) // not just an empty line // a line that isn't an element
		{
			write(2, COLOR_RED"Invalid Identifier\n"COLOR_RESET, 20);
			cleanup(cub, 1);
		}
		free_str_array(&cub->u_map.splited_line);
	}
	return (SUCCESS);
}

bool	only_space_so_empty(char *line) // only spaces
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

bool	lower_wall(char **map, int i)
{
	if (only_space_so_empty(map[i]))
		return (false);
	else if (i < str_ary_len(map) && only_space_so_empty(map[i + 1]))
		return (true);
	return (true);
}

bool	upper_wall(char **map, int i)
{
	if (only_space_so_empty(map[i]))
		return (false);
	else if (i > 0 && only_space_so_empty(map[i - 1]))
		return (true);
	return (true);
}

int	gap_in_wall(char **map, int i)
{
	//must check / cmp with previous and next line in map
	while ()
	{
		
	}
}

int	outside_checks(char **map, int i)
{
	if (upperwall(map, i) && gap_in_wall(map, i) == INV_MAP)
		return (INV_MAP);
	if (lower_wall(map, i) && gap_in_wall(map, i) == INV_MAP)
		return (INV_MAP);
	check_sides();
	return (SUCCESS);
}

int	closed_walls(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		if (outside_checks(map, i) == INV_MAP)
			return (INV_MAP);
	}
}

int	space_within_wall(char **map, int i, int j)
{
	//this is more complex then imagined
	
}

int	inv_space(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while(map[i][j])
		{
			if (map[i][j] == ' ' && space_within_wall(map, i, j))
				return (INV_MAP);
		}
		i++;
	}
}

int	check_map_element(t_main *cub, char *lines)
{	
	cub->u_map.map = ft_split(lines, '\n');
	if (!cub->u_map.map)
		return (perror("malloc"), FAILURE);
	if (closed_walls(cub->u_map.map) != SUCCESS)
	{
		write(2, COLOR_RED"Walls Not Closed!\n"COLOR_RESET, 30);
		return (INV_MAP);
	}
	if (inv_space(cub->u_map.map) != SUCCESS)
	{
		write(2, COLOR_RED"Whitespace Inside Map!\n"COLOR_RESET, 35);
		return (INV_MAP);
	}
	return (SUCCESS);
}

int	map_val(t_main *cub, char *map_path)
{
	cub->u_map.fd = open(map_path, O_RDONLY);
	if (cub->u_map.fd == -1)
		return (perror("open"), -1); // nothing to free / clean at this stage
	// read the 4 textures and 2 colors
	if (read_check_txts_clrs(cub) != SUCCESS)
		cleanup(cub, 1);
	//print_map_elements(&cub->u_map);
	if (read_map_element(cub) != SUCCESS)
		cleanup(cub, 2);
	check_map_element(cub, cub->u_map.joined_lines);
	return (SUCCESS);
}


/*

// not needed because even valid maps, separated by nl are invalid
	lines = add_generic_space(lines);
	if (!lines)
		return (perror(malloc), FAILURE);
int	ft_count_char(char *str, char c)
{
	int	counter;

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
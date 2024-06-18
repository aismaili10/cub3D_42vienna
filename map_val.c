/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:15 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/18 18:47:05 by aismaili         ###   ########.fr       */
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

/* int	closed_walls(char **map)
{
	int	i;
	int	j;

	i = 0;
	
	while (map[i])
	{
		if (map[i])
		
	}
}
*/
/* int	inv_whitespace(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if ()
	}
}  */

int	check_map_element(t_main *cub, char *lines)
{
	cub->u_map.map = ft_split(lines, '\n');
	if (!cub->u_map.map)
		return (perror("malloc"), FAILURE);
	/* if (closed_walls(cub->u_map.map) != SUCCESS)
	{
		write(2, COLOR_RED"Walls Not Closed!\n"COLOR_RESET, 30);
		return (INV_MAP);
	}
	if (inv_whitespace(cub->u_map.map) != SUCCESS)
	{
		write(2, COLOR_RED"Whitespace Inside Map!\n"COLOR_RESET, 35);
		return (INV_MAP);
	} */
	return (SUCCESS);
}

bool	is_cub_postfix(char *path)
{
	int	len;

	len = ft_strlen(path) - 1;
	if (len < 4)
		return (false);
	if (path[len] == 'b' && path[len - 1] == 'u'
		&& path[len - 2] == 'c' && path[len - 3] == '.')
		return (true);
	return (false);
}

int	map_val(t_main *cub, char *map_path)
{
	if (!is_cub_postfix(map_path))
		return (write (2, COLOR_RED"Invalid Map Postfix\n"COLOR_RESET, 31), -1);
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

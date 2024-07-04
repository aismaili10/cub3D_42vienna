/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:15 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/04 17:57:20 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int invalid_element(char *line)
{
	if (is_whitespace(line[0]) || is_whitespace(line[ft_strlen(line) - 1]))
	{
		write(2, COLOR_RED "Whitespace at Line-Beginning/End\n" COLOR_RESET, 45);
		return (INV_MAP);
	}
	return (SUCCESS);
}

int prep_for_init(char *line, t_main *cub)
{
	errno = 0;
	if (invalid_element(line) == INV_MAP)
		return (free(line), INV_MAP);
	cub->u_map.splited_line = ft_split_md(line, " \t");
	if (!cub->u_map.splited_line && errno)
		return (free(line), perror("malloc"), SYS_FAIL);
	if (str_ary_len(cub->u_map.splited_line) != 2 && ft_strncmp(line, "\n", 2))
	{
		write(2, COLOR_YELLOW "WARNING: Map Element Syntax: ", 37);
		write(2, "IDENTIFIER SPECIFIER\n" COLOR_RESET, 26);
		return (free(line), INV_MAP);
	}
	free(line);
	return (SUCCESS);
}

int read_check_txts_clrs(t_main *cub)
{
	char *tmp;

	errno = 0;
	while (!txts_clrs_found(&cub->u_map))
	{
		cub->u_map.id_ed = false;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
			cleanup(cub, 1);
		if (!tmp)
		{
			write(2, COLOR_RED "Incomplete Map File\n" COLOR_RESET, 32);
			cleanup(cub, 0);
		}
		if (prep_for_init(tmp, cub) != SUCCESS)
			cleanup(cub, 1);
		if (handle_color(cub) != SUCCESS || handle_texture(cub) != SUCCESS)
			cleanup(cub, 1);
		/* if (handle_texture(tmp, cub) != SUCCESS)
			cleanup(cub, 1); */
		if (!cub->u_map.id_ed && ft_strncmp("\n", cub->u_map.splited_line[0], 2)) // not just an empty line // a line that isn't an element
		{
			write(2, COLOR_RED "Invalid Identifier\n" COLOR_RESET, 31);
			cleanup(cub, 1);
		}
		free_str_array(&cub->u_map.splited_line);
	}
	return (SUCCESS);
}

int check_right(char *horiz, int c)
{
	int i;

	i = c;
	while (horiz[i])
	{
		if (horiz[i] == '1')
			return (true);
		if (ft_strchr("0NSWE", horiz[i]) && !ft_strchr("0NSWE1", horiz[i + 1])) // tricky here
		{
			printf("check_right: horiz[%i] = %c\n", i + 1, horiz[i + 1]);
			printf("check_right: horiz[%i] = %c\n", i, horiz[i]);
			return (false);
		}
		i++;
	}
	return (false);
}

int check_left(char *horiz, int c)
{
	int i;

	if (c < 1)
		return (false);
	i = c;
	while (i >= 0)
	{
		if (horiz[i] == '1')
			return (true);
		if (ft_strchr("0NSWE", horiz[i]) && (i == 0 || !ft_strchr("0NSWE1", horiz[i - 1])))
		{
			printf("check_left: horiz[%i] = %c\n", i - 1, horiz[i - 1]);
			printf("check_left: horiz[%i] = %c\n", i, horiz[i]);
			return (false);
		}
		i--;
	}
	return (false);
}

int check_up(char **vert, int r, int c)
{
	int i;

	i = r;
	while (i >= 0)
	{
		if (vert[i][c] == '1')
			return (true);
		if (ft_strchr("0NSWE", vert[i][c]) && (i == 0 || !ft_strchr("0NSWE1", vert[i - 1][c])))
		{
			printf("check_up: vert[%i][%i]: %c\n", i-1, c, vert[i-1][c]);
			printf("check_up: vert[%i][%i]: %c\n", i, c, vert[i][c]);
			return (false);
		}
		i--;
	}
	return (false);
}

int check_down(char **vert, int r, int c)
{
	int i;

	i = r;
	while (vert[i])
	{
		if (vert[i][c] == '1')
			return (true);
		if (ft_strchr("0NSWE", vert[i][c]) && (!vert[i + 1] || !ft_strchr("0NSWE1", vert[i + 1][c])))
		{
			printf("check_down: vert[%i][%i]: %c\n", i-1, c, vert[i-1][c]);
			printf("check_down: vert[%i][%i]: %c\n", i, c, vert[i][c]);
			printf("check_down: vert[%i][%i]: %c\n", i+1, c, vert[i+1][c]);
			return (false);
		}
		i++;
	}
	return (false);
}

int check_pos(char **map, int r, int c) // we send all 0s and the Player Position here
{
	if (!check_left(map[r], c))
	{
		printf("left: check_pos: map[%i]: -%s-\n", r, map[r]);
		return (INV_MAP);
	}
	if (!check_right(map[r], c))
	{
		printf("right: check_pos: map[%i]: -%s-\n", r, map[r]);
		return (INV_MAP);
	}
	if (!check_up(map, r, c))
	{
		printf("up: check_pos: map[%i]: -%s-\n", r, map[r]);
		return (INV_MAP);
	}
	if (!check_down(map, r, c))
	{
		printf("down: check_pos: map[%i]: -%s-\n", r, map[r]);
		return (INV_MAP);
	}
	return (SUCCESS);
}

int closed_walls(char **map)
{
	int i;
	int j;
	int p_counter;

	p_counter = 0;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (ft_strchr("0NSWE", map[i][j]) && check_pos(map, i, j) != SUCCESS)
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
		return (write(2, COLOR_RED "ERROR: PLAYER PLACEMENT\n" COLOR_RESET, 36), INV_MAP);
	}
	return (SUCCESS);
}

int get_max_len(char **map)
{
	int max;
	int r;

	r = 0;
	max = 0;
	while (map[r])
	{
		if (ft_strlen(map[r]) > (size_t)max)
			max = ft_strlen(map[r]);
		r++;
	}
	return (max);
}

int add_spaces(char *str, int max)
{
	int len;

	len = ft_strlen(str);
	while (len < max)
	{
		str[len] = ' ';
		len++;
	}
	str[len] = 0;
	return (SUCCESS);
}

int add_generic_spaces(char **map)
{
	char *temp;
	int r;
	int max_len;

	r = 0;
	max_len = get_max_len(map) + 1;
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

bool	has_only_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}

int	l_only_space(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (has_only_space(map[i]) /*&& and is not at the end */)
		{
			printf("l_only_space: map[%i]: %s\n", i, map[i]);
			write(2, COLOR_RED"LINE WITH ONLY SPACES IN 2D-MAP DETECTED\n"COLOR_RESET, 53);
			return (INV_MAP);
		}
		i++;
	}
	return (SUCCESS);
}

int check_map_element(t_main *cub, char *lines)
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

 // this function was gone for some reason
bool	is_cub_postfix(char *path)
{
	int		len;
	int		ary_len;
	char	**dirs_path;

	dirs_path = ft_split(path, '/');
	if (!dirs_path)
		return (perror("malloc"), false);
	ary_len = str_ary_len(dirs_path);
	if (ary_len > 1)
		len = ft_strlen(dirs_path[ary_len - 1]) - 1;
	else
		len = ft_strlen(path) - 1;
	if (len < 4)
		return (free_str_array(&dirs_path), false);
	if (dirs_path[ary_len - 1][len] == 'b' && dirs_path[ary_len - 1][len - 1] == 'u'
		&& dirs_path[ary_len - 1][len - 2] == 'c' && dirs_path[ary_len - 1][len - 3] == '.')
		return (free_str_array(&dirs_path), true);
	return (free_str_array(&dirs_path), false);
}

int	check_texture_path(t_main *cub)
{
	int fd;

	if (!cub->u_map.no || !cub->u_map.so || !cub->u_map.we || !cub->u_map.ea)
		return (write(2, RD "Texture Path Missing\n" CR, 32), INV_MAP);
	if ((fd = open(cub->u_map.no, O_RDONLY)) == -1)
		return (write(2, RD "Invalid path to NO texture\n" CR, 39), INV_MAP);
	else
		close(fd);
	if ((fd = open(cub->u_map.so, O_RDONLY)) == -1)
	return (write(2, RD "Invalid path to SO texture\n" CR, 39), INV_MAP);
	else
		close(fd);
	if ((fd = open(cub->u_map.we, O_RDONLY)) == -1)
	return (write(2, RD "Invalid path to WE texture\n" CR, 39), INV_MAP);
	else
		close(fd);
	if ((fd = open(cub->u_map.ea, O_RDONLY)) == -1)
	return (write(2, RD "Invalid path to EA texture\n" CR, 39), INV_MAP);
	else
		close(fd);
	return (0);
}

int map_val(t_main *cub, char *map_path)
{
	if (!is_cub_postfix(map_path))
		return (write(2, COLOR_RED"Invalid File Postfix\n"COLOR_RESET, 33), INV_MAP);
	cub->u_map.fd = open(map_path, O_RDONLY);
	if (cub->u_map.fd == -1)
		return (perror("open"), -1); // nothing to free / clean at this stage
	// read the 4 textures and 2 colors
	if (read_check_txts_clrs(cub) != SUCCESS)
		cleanup(cub, 1);
	// print_map_elements(&cub->u_map);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:46:26 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/07 11:07:41 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	is_whitespace(char c)
{
	if (c == 9 || (c >= 11 && c <= 13) || c == 32)
		return (true);
	return (false);
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
		if (has_only_space(map[i]))
		{
			write(2, RD"Error\nONLY SPACES LINE IN 2D-MAP DETECTED\n"CR, 54);
			return (INV_MAP);
		}
		i++;
	}
	return (SUCCESS);
}

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
	if (dirs_path[ary_len - 1][len] == 'b'
		&& dirs_path[ary_len - 1][len - 1] == 'u'
		&& dirs_path[ary_len - 1][len - 2] == 'c' &&
			dirs_path[ary_len - 1][len - 3] == '.')
		return (free_str_array(&dirs_path), true);
	return (free_str_array(&dirs_path), false);
}

int	check_texture_path(t_main *cub)
{
	int	fd;

	if (!cub->u_map.no || !cub->u_map.so || !cub->u_map.we || !cub->u_map.ea)
		return (write(2, RD "Error\nTexture Path Missing\n" CR, 37), INV_MAP);
	fd = open(cub->u_map.no, O_RDONLY);
	if (fd == -1)
		return (write(2, RD "Error\nInvalid path to NO texture\n" CR, 45), -2);
	else
		close(fd);
	fd = open(cub->u_map.so, O_RDONLY);
	if (fd == -1)
		return (write(2, RD "Error\nInvalid path to SO texture\n" CR, 45), -2);
	else
		close(fd);
	fd = open(cub->u_map.we, O_RDONLY);
	if (fd == -1)
		return (write(2, RD "Error\nInvalid path to WE texture\n" CR, 45), -2);
	else
		close(fd);
	fd = open(cub->u_map.ea, O_RDONLY);
	if (fd == -1)
		return (write(2, RD "Error\nInvalid path to EA texture\n" CR, 45), -2);
	else
		close(fd);
	return (0);
}

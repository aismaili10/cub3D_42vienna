/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:39:02 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/07 11:01:56 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	invalid_element(char *line)
{
	if (ft_strchr(line, '\t'))
	{
		write(2, RD "Error\nTab inside Config-File\n" CR, 41);
		return (INV_MAP);
	}
	return (SUCCESS);
}

bool	is_val_ary_len(t_main *cub)
{
	int	ary_len;

	ary_len = str_ary_len(cub->u_map.spl_ln);
	if (ary_len != 2)
	{
		if (ary_len == 3 && !ft_strncmp(cub->u_map.spl_ln[2], "\n", 2))
			return (true);
		else
			return (false);
	}
	return (true);
}

int	prep_for_init(char *line, t_main *cub)
{
	errno = 0;
	if (invalid_element(line) == INV_MAP)
		return (free(line), INV_MAP);
	cub->u_map.spl_ln = ft_split(line, ' ');
	if (!cub->u_map.spl_ln && errno)
		return (free(line), perror("malloc"), SYS_FAIL);
	if (!is_val_ary_len(cub) && ft_strncmp(line, "\n", 2))
	{
		write(2, CY"Error\nMap Element Syntax: ", 34);
		write(2, "IDENTIFIER SPECIFIER\n" CR, 26);
		return (free(line), INV_MAP);
	}
	free(line);
	return (SUCCESS);
}

int	read_check_txts_clrs(t_main *cub)
{
	char	*tmp;

	while (!txts_clrs_found(&cub->u_map))
	{
		cub->u_map.id_ed = false;
		errno = 0;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
			cleanup(cub, 2);
		if (!tmp)
		{
			write(2, RD "Error\nIncomplete Map File\n" CR, 38);
			cleanup(cub, 2);
		}
		if (prep_for_init(tmp, cub) != SUCCESS || handle_color(cub) != SUCCESS
			|| handle_texture(cub) != SUCCESS)
			cleanup(cub, -1);
		if (!cub->u_map.id_ed && ft_strncmp("\n", cub->u_map.spl_ln[0], 2))
		{
			write(2, RD "Error\nInvalid Identifier\n" CR, 37);
			cleanup(cub, -1);
		}
		free_str_array(&cub->u_map.spl_ln);
	}
	return (SUCCESS);
}

int	get_max_len(char **map)
{
	int	max;
	int	r;

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

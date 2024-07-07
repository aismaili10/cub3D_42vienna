/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_element2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 13:49:14 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/07 11:04:41 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	inv_char_in_map(char c)
{
	if (!ft_strchr("\n10NSWE ", c))
	{
		write(2, RD"Error\nInvalid Character in Map\n"CR, 43);
		return (INV_MAP);
	}
	return (SUCCESS);
}

bool	map_ended(char *str, int i)
{
	while (str[i])
	{
		if (str[i] != '\n' && str[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}

bool	nl_nl(char *line, int i)
{
	if (line[i] == '\n' && line[i + 1] == '\n')
		return (true);
	return (false);
}

int	nline(char *n_lines)
{
	int	i;
	int	line_counter;

	i = 0;
	line_counter = 0;
	while (n_lines[i] == '\n')
		i++;
	while (n_lines[i])
	{
		if (inv_char_in_map(n_lines[i]) == INV_MAP)
			return (INV_MAP);
		if (nl_nl(n_lines, i) && !map_ended(n_lines, i))
		{
			write(2, RD"Error\nEmpty Line inside Map\n"CR, 40);
			return (INV_MAP);
		}
		i++;
		if (n_lines[i] == '\n')
			line_counter++;
	}
	return (SUCCESS);
}

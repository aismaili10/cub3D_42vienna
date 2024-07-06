/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_rm_top_bottom.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 13:52:14 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/06 14:06:59 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	first_map_char(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ft_strchr("10NSWE", line[i]))
		{
			while (i >= 0 && line[i] != '\n')
				i--;
			return (i + 1);
		}
		i++;
	}
	return (i);
}

int	last_map_char(char *line)
{
	int	i;

	i = ft_strlen(line) - 1;
	while (i >= 0)
	{
		if (ft_strchr("10NSWE", line[i]))
			return (i);
		i--;
	}
	return (i);
}

char	*rm_empty_top_bottom(char *line)
{
	char	*n_line;
	int		first;
	int		last;

	first = first_map_char(line);
	last = last_map_char(line) + 1;
	n_line = ft_substr(line, first, last - first);
	if (!n_line)
		return (free(line), NULL);
	free(line);
	return (n_line);
}

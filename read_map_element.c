/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_element.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:03:20 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/06 13:52:33 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

bool	contains_inv_char(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
	{
		if (!ft_strchr("10NSWE \n", str[i]))
		{
			write(2, RD"LINE WITH INVALID CHAR\n"CR, 35);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	contains_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
	{
		if (str[i] != ' ')
			return (false);
		i++;
	}
	if (i == 0 && str[i] == '\n')
		return (false);
	write(2, RD"LINE WITH ONLY SPACES\n"CR, 34);
	return (true);
}

void	go_to_cleanup(t_main *cub, char *tmp)
{
	free(tmp);
	get_next_line(cub->u_map.fd, 1);
	cleanup(cub, 2);
}

int	read_map_element(t_main *cub)
{
	char	*tmp;

	while (1)
	{
		errno = 0;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
			cleanup(cub, 2);
		if (!tmp)
			break ;
		if (contains_only_spaces(tmp) || contains_inv_char(tmp))
			go_to_cleanup(cub, tmp);
		cub->u_map.joined_lines = ft_strjoingnl(cub->u_map.joined_lines, tmp);
		if (!cub->u_map.joined_lines)
			go_to_cleanup(cub, tmp);
		free(tmp);
	}
	cub->u_map.joined_lines = rm_empty_top_bottom(cub->u_map.joined_lines);
	if (!cub->u_map.joined_lines)
		return (FAILURE);
	return (nline(cub->u_map.joined_lines));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_element.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:03:20 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/18 21:17:28 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	inv_char_in_map(char c)
{
	if (!ft_strchr("\n10NSWE ", c)) // anything besides valid ones
	{
		write(2, COLOR_RED"Invalid Character in Map\n"COLOR_RESET, 37);
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

bool	empty_line(char *line, int i)
{
	if (line[i] == '\n' && line[i + 1] == '\n')
		return (true);
	return (false);
}

int	nline(char *one_line)
{
	int	i;
	int	line_counter;

	i = 0;
	line_counter = 0;
	while (one_line[i] == '\n')
		i++;
	while (one_line[i])
	{
		if (empty_line(one_line, i) && !map_ended(one_line, i))
		{
			//printf("i value in nline: %i\n", i);
			write(2, COLOR_RED"Empty Line inside Map\n"COLOR_RESET, 34);
			return (INV_MAP);
		}
		if (inv_char_in_map(one_line[i]) == INV_MAP)
			return (INV_MAP);
		i++;
		if (one_line[i] == '\n')
			line_counter++;
	}
	//printf("line_counter value in nline: %i\n", line_counter);
	if (line_counter < 3)
	{
		write(2, COLOR_RED"Less than 3 Lines In Map\n"COLOR_RESET, 37);
		return (INV_MAP);
	}
	return (SUCCESS);
}

int	read_map_element(t_main *cub)
{
	char	*tmp;
	int		gnl_errno;

	while (1)
	{
		gnl_errno = errno;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && gnl_errno)
		{
			get_next_line(cub->u_map.fd, 1);
			cleanup(cub, 1);
		}
		if (!tmp)
			break ;
		cub->u_map.joined_lines = ft_strjoingnl(cub->u_map.joined_lines, tmp);
		if (!cub->u_map.joined_lines)
		{
			free(tmp);
			cleanup(cub, 1);
		}
		free(tmp);
	}
	return (nline(cub->u_map.joined_lines));
}

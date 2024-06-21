/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_element.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:03:20 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/21 14:16:05 by aismaili         ###   ########.fr       */
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
		if (str[i] != '\n' && str[i] != ' ') // is not nl or space
		{
			printf("str: %s and pos: str[%i]: %c\n", str, i, str[i]);
			return (false);
		}
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

int	first_map_char(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ft_strchr("10NSWE", line[i]))
		{
			// retract to the previous newline
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
	int	first;
	int	last;

	first = first_map_char(line);
	last = last_map_char(line) + 1;

	// printf("length: %zu\nfirst: %i\n last: %i\n", ft_strlen(line), first, last);
	char	*n_line = ft_substr(line, first, last - first);
	if (!n_line)
		return (free(line), NULL);
	// printf("line:\n%s\n", line);
	// printf("n_line:\n%s\n", n_line);
	free(line);
	return (n_line);
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
			printf("i value in nline: n_lines[%i]: %c\n", i, n_lines[i]);
			write(2, COLOR_RED"Empty Line inside Map\n"COLOR_RESET, 34);
			return (INV_MAP);
		}
		i++;
		if (n_lines[i] == '\n')
			line_counter++;
	}
	//printf("line_counter value in nline: %i\n", line_counter);
	/* if (line_counter < 3) // this check is not enough to exclude to small maps
	{
		write(2, COLOR_RED"Less than 3 Lines In Map\n"COLOR_RESET, 37);
		return (INV_MAP);
	} */
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
	cub->u_map.joined_lines = rm_empty_top_bottom(cub->u_map.joined_lines);
	if (!cub->u_map.joined_lines)
		return (FAILURE);
	return (nline(cub->u_map.joined_lines));
}

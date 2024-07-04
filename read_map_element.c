/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_element.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:03:20 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/04 19:07:18 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	inv_char_in_map(char c)
{
	if (!ft_strchr("\n10NSWE ", c))
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
	return (SUCCESS);
}

bool	contains_inv_char(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
	{
		if (!ft_strchr("10NSWE \n", str[i]))
		{
		write(2, COLOR_RED"LINE WITH INVALID CHAR\n"COLOR_RESET, 35);
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
	write(2, COLOR_RED"LINE WITH ONLY SPACES\n"COLOR_RESET, 34);
	return (true);
}

void	go_to_cleanup(t_main *cub, char *tmp)
{
	free(tmp);
	cleanup(cub, 1);
}

int	read_map_element(t_main *cub)
{
	char	*tmp;

	while (1)
	{
		errno = 0;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
			cleanup(cub, -1); // -1: free remaining in get_next_line()
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

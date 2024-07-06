/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:42:42 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/06 14:06:10 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	check_right(char *horiz, int c)
{
	int	i;

	i = c;
	while (horiz[i])
	{
		if (horiz[i] == '1')
			return (true);
		if (ft_strchr("0NSWE", horiz[i]) && !ft_strchr("0NSWE1", horiz[i + 1]))
		{
			write(2, RD "Invalid 2D Map\n" CR, 27);
			return (false);
		}
		i++;
	}
	return (false);
}

int	check_left(char *horiz, int c)
{
	int	i;

	if (c < 1)
		return (false);
	i = c;
	while (i >= 0)
	{
		if (horiz[i] == '1')
			return (true);
		if (ft_strchr("0NSWE", horiz[i]) && (i == 0
				|| !ft_strchr("0NSWE1", horiz[i - 1])))
		{
			write(2, RD "Invalid 2D Map\n" CR, 27);
			return (false);
		}
		i--;
	}
	return (false);
}

int	check_up(char **vert, int r, int c)
{
	int	i;

	i = r;
	while (i >= 0)
	{
		if (vert[i][c] == '1')
			return (true);
		if (ft_strchr("0NSWE", vert[i][c]) &&
			(i == 0 || !ft_strchr("0NSWE1", vert[i - 1][c])))
		{
			write(2, RD "Invalid 2D Map\n" CR, 27);
			return (false);
		}
		i--;
	}
	return (false);
}

int	check_down(char **vert, int r, int c)
{
	int	i;

	i = r;
	while (vert[i])
	{
		if (vert[i][c] == '1')
			return (true);
		if (ft_strchr("0NSWE", vert[i][c]) &&
			(!vert[i + 1] || !ft_strchr("0NSWE1", vert[i + 1][c])))
		{
			write(2, RD "Invalid 2D Map\n" CR, 27);
			return (false);
		}
		i++;
	}
	return (false);
}

int	check_pos(char **map, int r, int c)
{
	if (!check_left(map[r], c))
	{
		return (INV_MAP);
	}
	if (!check_right(map[r], c))
	{
		return (INV_MAP);
	}
	if (!check_up(map, r, c))
	{
		return (INV_MAP);
	}
	if (!check_down(map, r, c))
	{
		return (INV_MAP);
	}
	return (SUCCESS);
}

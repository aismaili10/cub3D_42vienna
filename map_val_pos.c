/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val_pos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:42:42 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/05 15:00:11 by aszabo           ###   ########.fr       */
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
			printf("check_left: horiz[%i] = %c\n", i - 1, horiz[i - 1]);
			printf("check_left: horiz[%i] = %c\n", i, horiz[i]);
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
			printf("check_up: vert[%i][%i]: %c\n", i - 1, c, vert[i - 1][c]);
			printf("check_up: vert[%i][%i]: %c\n", i, c, vert[i][c]);
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
			printf("check_down: vert[%i][%i]: %c\n", i - 1, c, vert[i - 1][c]);
			printf("check_down: vert[%i][%i]: %c\n", i, c, vert[i][c]);
			printf("check_down: vert[%i][%i]: %c\n", i + 1, c, vert[i + 1][c]);
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
		printf("left: check_pos: map[%i]: -%s-\n", r, map[r]);//do we need these prints??
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

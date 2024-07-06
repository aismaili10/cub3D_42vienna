/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_val_xpm.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 20:05:43 by aszabo            #+#    #+#             */
/*   Updated: 2024/07/06 20:28:27 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	*get_file_name(char *path)
{
	int		i;
	int		j;
	char	*file_name;

	i = 0;
	j = 0;
	while (path[i])
	{
		if (path[i] == '/')
			j = i + 1;
		i++;
	}
	file_name = ft_strdup(&path[j]);
	if (!file_name)
		return (NULL);
	return (file_name);
}

int	check_xpm_ending(char *path)
{
	int		len;
	char	*file_name;

	file_name = get_file_name(path);
	len = ft_strlen(file_name);
	if (len < 5)
		return (free(file_name), INV_MAP);
	if (file_name[len - 1] != 'm' || file_name[len - 2] != 'p'
		|| file_name[len - 3] != 'x' || file_name[len - 4] != '.')
		return (free(file_name), INV_MAP);
	return (free(file_name), SUCCESS);
}

int	check_xpm_extension(t_main *cub)
{
	if (check_xpm_ending(cub->u_map.no) != SUCCESS)
	{
		write(2, RD "Error\nInvalid Texture Extension\n" CR, 43);
		return (INV_MAP);
	}
	if (check_xpm_ending(cub->u_map.so) != SUCCESS)
	{
		write(2, RD "Error\nInvalid Texture Extension\n" CR, 43);
		return (INV_MAP);
	}
	if (check_xpm_ending(cub->u_map.ea) != SUCCESS)
	{
		write(2, RD "Error\nInvalid Texture Extension\n" CR, 43);
		return (INV_MAP);
	}
	if (check_xpm_ending(cub->u_map.we) != SUCCESS)
	{
		write(2, RD "Error\nInvalid Texture Extension\n" CR, 43);
		return (INV_MAP);
	}
	return (SUCCESS);
}

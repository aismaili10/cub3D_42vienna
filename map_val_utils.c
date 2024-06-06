#include "cub3D.h"

int	str_array_len(char **str_array)
{
	int	i;

	i = 0;
	if (!str_array)
		return (0);
	while (str_array[i])
		i++;
	return (i);
}

void	free_str_array(char ***str_array)
{
	int	i;

	i = -1;
	if (!*str_array)
		return ;
	while ((*str_array)[++i])
	{
		free((*str_array)[i]);
		(*str_array)[i] = NULL;
	}
	free(*str_array);
	*str_array = NULL;
}

bool	is_whitespace(char c)
{
	if (c == 9 || (c >= 11 && c <= 13) || c == 32)
		return (true);
	return (false);
}

static bool	found_color(t_map *u_map)
{
	if (u_map->c_color != -1 && u_map->f_color != -1)
		return (true);
	return (false);
}

static bool	found_textures(t_map *u_map)
{
	if (u_map->no != 0 && u_map->so != 0
		&& u_map->we != 0 && u_map->ea != 0)
		return (true);
	return (false);
}

bool	txts_clrs_found(t_map *u_map)
{
	if (found_color(u_map) && found_textures(u_map))
		return (true);
	return (false);
}

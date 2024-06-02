#include "cub3D.h"

int	init_u_map_clr(char *line, t_map *u_map)
{
	char	**str_rgb;
	int		rgb[3];
	int		i;

	str_rgb = ft_split(u_map->splited_line[1], ',');
	if (!str_rgb)
		return (SYS_FAIL);
	i = -1;
	while (++i < 3)
		rgb[i] = ft_atoi(str_rgb[i]);
	free_str_array(str_rgb);
	i = ft_strlen(u_map->splited_line[0]);
	if (!ft_strncmp(u_map->splited_line[0], "C", i))
		u_map->c_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2]; // gotta understand how exactly that works
	else if (!ft_strncmp(u_map->splited_line[0], "F", i))
		u_map->c_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	return (SUCCESS);
}


// sp_line is u_map.splited_line
int	init_u_map_txts(char **sp_line, t_map *u_map) // we could/should refactor this --> implement an array for the textures with enums as index and loop that way, maybe
{
	int	len;

	len = ft_strlen(sp_line[0]);
	if (!ft_strnchr(sp_line[0], "NO", len))
	{
		u_map->no = ft_strdup(sp_line[1]);
		if (!u_map->no)
			return (SYS_FAIL);
	}
	else if (!ft_strnchr(sp_line[0], "SO", len))
	{
		u_map->so = ft_strdup(sp_line[1]);
		if (!u_map->so)
			return (SYS_FAIL);
	}
	else if (!ft_strnchr(sp_line[0], "WE", len))
	{
		u_map->we = ft_strdup(sp_line[1]);
		if (!u_map->we)
			return (SYS_FAIL);
	}
	else if (!ft_strnchr(sp_line[0], "EA", len))
	{
		u_map->ea = ft_strdup(sp_line[1]);
		if (!u_map->ea)
			return (SYS_FAIL);
	}
	return (SUCCESS);
}

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (true);
	return (false);
}

int	invalid_element(char *line)
{
	if (is_whitespace(line[0]) || is_whitespace(line[ft_strlen(line) - 1]))
		return (write(2, "whitespace at beginning/end of line\n", 37), INV_MAP);
}

bool	found_color(t_map *u_map)
{
	if (u_map->c_color != -1 && u_map->f_color != -1)
		return (true);
	return (false);
}

bool	found_textures(t_map *u_map)
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
int	is_color(char *id, t_map *u_map)
{
	int	len;

	len = ft_strlen(id);
	if (!ft_strncmp(id, "C", len) && u_map->c_color == -1)
		return (true);
	else if (!ft_strncmp(id, "C", len) && u_map->c_color != -1)
		return (INV_MAP);
	if (!ft_strncmp(id, "F", len) && u_map->f_color == -1)
		return (true);
	else if (!ft_strncmp(id, "F", len) && u_map->f_color != -1)
		return (INV_MAP);
	return (false);
}

int	handle_color(char *line, t_main *cub)
{
	int	ret;

	ret = is_color(cub->u_map.splited_line[0], &cub->u_map);
	if (ret == INV_MAP)
		return (INV_MAP);
	if (ret)
		return (init_u_map_clr(line, &cub->u_map));
	return (SUCCESS);
}

int	is_texture(char *type, t_map *u_map)
{
	int	len;

	len = ft_strlen(type);
	if (!ft_strncmp(type, "NO", len) && !u_map->no)
		return (true);
	else if (!ft_strncmp(type, "NO", len) && u_map->no)
		return (INV_MAP);
	if (!ft_strncmp(type, "SO", len) && !u_map->so)
		return (true);
	else if (!ft_strncmp(type, "SO", len) && u_map->so)
		return (INV_MAP);
	if (!ft_strncmp(type, "WE", len) && !u_map->we)
		return (true);
	else if (!ft_strncmp(type, "WE", len) && u_map->we)
		return (INV_MAP);
	if (!ft_strncmp(type, "EA", len) && !u_map->ea)
		return (true);
	else if (!ft_strncmp(type, "EA", len) && u_map->ea)
		return (INV_MAP);
	return (false);
}

int	handle_texture(char *line, t_main *cub)
{
	int	ret;

	ret = is_texture(cub->u_map.splited_line[0], &cub->u_map);
	if (ret == INV_MAP)
		return (INV_MAP);
	if (ret)
		return (init_u_map_txts(cub->u_map.splited_line, &cub->u_map));
	return (SUCCESS);
}

int	prep_for_init(char *line, t_main *cub)
{
	errno = 0;
	if (invalid_element(line) == INV_MAP)
		return (INV_MAP);
	cub->u_map.splited_line = ft_split(line, ' '); // adjust for all whitespaces
	if (!cub->u_map.splited_line && errno)
		return (perror("malloc"), SYS_FAIL);
	if (str_array_len(cub->u_map.splited_line) > 2)
		return (INV_MAP);
	return (SUCCESS);
}

int	map_val(t_main *cub, char *map_path)
{
	// file name/type checks??
	// path checks??
	// !!!if the file doesn't exist, open should fail!!!
	char	*tmp;
	//int 	ret;

	cub->u_map.fd = open(map_path, O_RDONLY);
	if (cub->u_map.fd == -1)
		return (perror("open"), -1); // nothing to free / clean at this stage
	// read the 4 textures and 2 colors
	errno = 0;
	while (!txts_clrs_found(&cub->u_map))
	{
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
		{
			// start cleaning and exit process
			cleanup(cub, 1);
		}
		// if anything besides the first 6 map elements --> INVALID
		if (prep_for_init(tmp, cub) != SUCCESS)
			cleanup(cub, 1);
		if (handle_color(tmp, cub) != SUCCESS)
			cleanup(cub, 1);
		if (!handle_texture(tmp, cub) != SUCCESS)
			cleanup(cub, 1);
		else if (ft_strncmp(tmp, "\n", ft_strlen(tmp))) // not just an empty line // a line that isn't an element
		{
			write(2, "Invalid Identifier\n", 20);
			return (FAILURE);
		}
		free(tmp);
	}
	// read to fill cub->u_map.map
	while (1)
	{
		tmp = get_next_line(cub->u_map.fd, 1);
		if (!tmp && errno)
		{
			// handle malloc fail
		}
		// 
	}
	
	return (SUCCESS);
}

/*
	!!!! cases to be aware of !!!!
	line starting with spaces
	line ending with spaces
	line with more then just the element identifier and specifier
	lines with only whitespaces


	++ elements could come up multiple times --> ERROR ++


*/
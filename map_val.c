#include "cub3D.h"

int	init_u_map_clr(char *line, t_map *u_map)
{
	char	**str_rgb;
	int		rgb[3];
	int		i;

	(void)line;
	errno = 0;
	str_rgb = ft_split(u_map->splited_line[1], ',');
	if (!str_rgb && errno)
		return (perror("malloc"), SYS_FAIL);
	if (str_array_len(str_rgb) != 3)
		return (write(2, COLOR_RED"RGB Invalid Map\n"COLOR_RESET, 28), INV_MAP);
	i = -1;
	while (++i < 3)
		rgb[i] = ft_atoi(str_rgb[i]); // handle overflow!!
	free_str_array(&str_rgb);
	i = ft_strlen(u_map->splited_line[0]);
	if (!ft_strncmp(u_map->splited_line[0], "C", i))
		u_map->c_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2]; // gotta understand how exactly that works
	else if (!ft_strncmp(u_map->splited_line[0], "F", i))
		u_map->f_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	u_map->id_ed = true;
	return (SUCCESS);
}

// sp_line is u_map.splited_line
// we could/should refactor this --> implement an array for the textures with enums as index and loop that way, maybe
int	init_u_map_txts(char **sp_line, t_map *u_map)
{
	int	len;

	//printf("in ini_u_map_txts\n");
	char *tmp = ft_strdup(sp_line[1]);
	if (!tmp)
			return (SYS_FAIL);
	len = ft_strlen(sp_line[0]);
	if (!ft_strncmp(sp_line[0], "NO", len))
		u_map->no = tmp;
	else if (!ft_strncmp(sp_line[0], "SO", len))
		u_map->so = tmp;
	else if (!ft_strncmp(sp_line[0], "WE", len))
		u_map->we = tmp;
	else if (!ft_strncmp(sp_line[0], "EA", len))
		u_map->ea = tmp;
	else
		return (free(tmp), INV_MAP);
	u_map->id_ed = true;
	//printf("id_ed set tuo true\n");
	return (SUCCESS);
}

int	invalid_element(char *line)
{
	if (is_whitespace(line[0]) || is_whitespace(line[ft_strlen(line) - 1]))
		return (write(2, COLOR_RED"whitespace at beginning/end of line\n"COLOR_RESET, 48), INV_MAP);
	return (SUCCESS);
}


int	is_color(char *id, t_map *u_map)
{
	int	len;

	len = ft_strlen(id);
	//printf("id: %s\n", id);
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
	{
		return (write(2, COLOR_RED"C/F Color Dublicate encountered\n"COLOR_RESET, 44), INV_MAP);
	}
	if (ret)
	{
		return (init_u_map_clr(line, &cub->u_map));
	}
	return (SUCCESS);
}

int	is_texture(char *type, t_map *u_map)
{
	//int	len;

	//len = ft_strlen(type);//NOPE
	//printf("in is_texture\n");
	if (!ft_strncmp(type, "NO", 3) && !u_map->no)
		return (true);
	else if (!ft_strncmp(type, "NO", 3) && u_map->no)
		return (INV_MAP);
	if (!ft_strncmp(type, "SO", 3) && !u_map->so)
		return (true);
	else if (!ft_strncmp(type, "SO", 3) && u_map->so)
		return (INV_MAP);
	if (!ft_strncmp(type, "WE", 3) && !u_map->we)
		return (true);
	else if (!ft_strncmp(type, "WE", 3) && u_map->we)
		return (INV_MAP);
	if (!ft_strncmp(type, "EA", 3) && !u_map->ea)
		return (true);
	else if (!ft_strncmp(type, "EA", 3) && u_map->ea)
		return (INV_MAP);
	return (false);
}

int	handle_texture(char *line, t_main *cub)
{
	int	ret;

	(void)line;
	if (cub->u_map.id_ed)
		return (SUCCESS);
	ret = is_texture(cub->u_map.splited_line[0], &cub->u_map);
	if (ret == INV_MAP)
		return (write(2, COLOR_RED"WALL Texture Dublicate encountered\n"COLOR_RESET, 47), INV_MAP);
	if (ret == true)
		return (init_u_map_txts(cub->u_map.splited_line, &cub->u_map));
	return (SUCCESS);
}

int	prep_for_init(char *line, t_main *cub)
{
	errno = 0;
	if (invalid_element(line) == INV_MAP)
		return (INV_MAP);
	cub->u_map.splited_line = ft_split_md(line, " \t"); // todo: test the new split
	if (!cub->u_map.splited_line && errno)
		return (perror("malloc"), SYS_FAIL);
	if (str_array_len(cub->u_map.splited_line) != 2 && ft_strncmp(line, "\n", ft_strlen(line)))
		return (write(2, COLOR_YELLOW"warning: map element syntax: IDENTIFIER SPECIFIER\n"COLOR_RESET, 61), INV_MAP);
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
	//int	i = 0;
	while (!txts_clrs_found(&cub->u_map))
	{
		//printf("in loop: i: %i\n", i++);
		cub->u_map.id_ed = false;
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
		{
			// start cleaning and exit process
			//get_next_line(cub->u_map.fd, 1);
			cleanup(cub, -1);
		}
		if (!tmp)
		{
			write(2, COLOR_RED"Incomplete Map file\n"COLOR_RESET, 32);
			cleanup(cub, 0);
		}
		// if anything besides the first 6 map elements --> INVALID
		if (prep_for_init(tmp, cub) != SUCCESS)
			return (free(tmp), cleanup(cub, 1), -2);
		if (handle_color(tmp, cub) != SUCCESS)
			return (free(tmp), cleanup(cub, 2), -2);
		if (handle_texture(tmp, cub) != SUCCESS)
			return (free(tmp), cleanup(cub, 3), -2);
		if (!cub->u_map.id_ed && ft_strncmp("\n", tmp, ft_strlen(tmp))) // not just an empty line // a line that isn't an element
		{
			printf("tmp: %s\n", tmp);
			write(2, COLOR_RED"Invalid Identifier\n"COLOR_RESET, 20);
			free(tmp);
			cleanup(cub, 4);
		}
		free_str_array(&cub->u_map.splited_line);
		free(tmp);
	}
	//print_map_elements(&cub->u_map);

	// last p
	/*while (1)
	{
		tmp = get_next_line(cub->u_map.fd, 0);
		if (!tmp && errno)
		{
			// handle malloc fail
			get_next_line(cub->u_map.fd, 1);

		}
		// if anything besides map characters --> INV_MAP
		// two or more newlines in series --> INV_MAP
		// 
	}*/
	
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
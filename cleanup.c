#include "cub3D.h"

void	free_textures(t_map *u_map)
{
	if (u_map->no)
		free(u_map->no);
	if (u_map->so)
		free(u_map->so);
	if (u_map->we)
		free(u_map->we);
	if (u_map->ea)
		free(u_map->ea);
}

void	free_map(char **map)
{
	int	i;

	i = -1;
	if (!map)
		return ;
	while (map[++i])
		free(map[i]);
	free(map);
}

void	free_map_elements(t_main *cub)
{
	if (cub->u_map.fd != -1)
		close(cub->u_map.fd);
	free_textures(&cub->u_map);
	free_str_array(&cub->u_map.splited_line);
	free_map(cub->u_map.map);
}

void	cleanup(t_main *cub, int stage)
{
	(void)cub;
	(void)stage;
	printf("in cleanup; stage: %i\n", stage);
	print_map_elements(&cub->u_map);
	free_map_elements(cub);
	exit(1);
	/*if (stage == 1)
		cleanup_1();
	if (stage == 2)
		cleanup_2();*/
	// ...
}

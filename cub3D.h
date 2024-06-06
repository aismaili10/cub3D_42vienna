/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:49:02 by aismaili          #+#    #+#             */
/*   Updated: 2024/05/31 16:04:00 by aismaili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CUB3D_H
#define	CUB3D_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
//#include <mlx.h>

#include "libft/libft.h"
#include "libft/gnl/get_next_line.h"
#include "libft/printf/ft_printf.h"

// return values
#define	INV_MAP		-2
#define	SYS_FAIL	-1
#define	SUCCESS		0
#define	FAILURE		1


// colors
#define COLOR_GREEN		"\033[0;32m"	// len: 7
#define COLOR_RED		"\033[0;31m"	// len: 7
#define COLOR_YELLOW	"\033[0;33m"	// len: 7
#define COLOR_RESET		"\033[0m"		// len: 4
typedef	struct	s_map
{
	int		fd; // close directly after done with the map file
	char	**map; // !! should be null terminated to free easily
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		f_color;
	int		c_color;
	char	**splited_line; // free diretly after no using anymore
	bool	id_ed;
}	t_map;

typedef struct	s_main // maybe change name to s_cub3d?? aligns more with the project
{
	// mlx init and window
	t_map	u_map;
}	t_main;

// cleanup
void		cleanup(t_main *cub, int stage);

// map_val
int		map_val(t_main *cub, char *map_path);

// map_val_utils
int		str_array_len(char **str_array);
void	free_str_array(char ***str_array);
bool	is_whitespace(char c);
bool	txts_clrs_found(t_map *u_map);

// debuging
void	print_map_elements(t_map* u_map);

#endif
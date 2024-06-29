/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aismaili <aismaili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:49:02 by aismaili          #+#    #+#             */
/*   Updated: 2024/06/29 13:39:21 by aismaili         ###   ########.fr       */
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
#include <mlx.h>
#include <math.h>
#include <X11/keysym.h>
#include <X11/X.h>

#include "libft/libft.h"
#include "libft/gnl/get_next_line.h"
#include "libft/printf/ft_printf.h"

// return values
#define	INV_MAP		-2
#define	SYS_FAIL	-1
#define	SUCCESS		0
#define	FAILURE		1

#define WIN_WIDTH	800
#define WIN_HEIGHT	600
#define KEY_ESC 65307
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_LEFT 65363
#define KEY_RIGHT 65361
#define screenWidth 800
#define screenHeight 600
#define MINIMAP_SCALE 5
#define MINIMAP_SIZE 100
#define ROT_SPEED 0.05 // Rotation speed in radians
#define MOVE_SPEED 0.2 // Movement speed
#define CEILING_COLOR 0x87CEEB  // Light blue for the ceiling
#define FLOOR_COLOR 0x008000  


// colors
#define COLOR_GREEN		"\033[0;32m"	// len: 7
#define COLOR_RED		"\033[0;31m"	// len: 7
#define COLOR_YELLOW	"\033[0;33m"	// len: 7
#define COLOR_RESET		"\033[0m"		// len: 4

// keycode
#define	ESC		65307

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
	char	**splited_line; // free diretly after no using anymore // maybe better name
	char	*joined_lines;
	bool	id_ed;
}	t_map;

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bpp; //bits_per_pixel
	int		line_length;
	int		endian;
}	t_img;

typedef struct s_player {
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} t_player;

typedef struct	s_main // maybe change name to s_cub3d?? aligns more with the project
{
	// mlx init and window
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	mlx_img;
	t_map	u_map;
	t_player *player;
}	t_main;

// cleanup
void		cleanup(t_main *cub, int stage);

// map_val
int		map_val(t_main *cub, char *map_path);
int		handle_color(char *line, t_main *cub);
int		handle_texture(char *line, t_main *cub);
int		read_map_element(t_main *cub);

// game
int game(t_main *cub);

// map_val_utils
int		str_ary_len(char **str_array);
void	free_str_array(char ***str_array);
bool	is_whitespace(char c);
bool	txts_clrs_found(t_map *u_map);

//mlx_init
int		init_mlx(t_main *cub);

// debuging
void	print_map_elements(t_map* u_map);

#endif
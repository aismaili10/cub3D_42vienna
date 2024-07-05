/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aszabo <aszabo@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 12:49:02 by aismaili          #+#    #+#             */
/*   Updated: 2024/07/05 15:15:24 by aszabo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>
# include <mlx.h>
# include <math.h>
# include <X11/keysym.h>
# include <X11/X.h>

# include "libft/libft.h"
# include "libft/gnl/get_next_line.h"
# include "libft/printf/ft_printf.h"

// RETURN VALUES
# define INV_MAP		-2
# define SYS_FAIL	-1
# define SUCCESS		0
# define FAILURE		1

# define WIN_WIDTH	800
# define WIN_HEIGHT	600
# define TEX_WIDTH	64
# define TEX_HEIGHT	64

# define ROT_SPEED 0.005 // Rotation speed in radians
# define MOVE_SPEED 0.01 // Movement speed
// #define ROT_SPEED 0.1 // Rotation speed in radians
// #define MOVE_SPEED 0.1 // Movement speed

// MINIMAP
# define MINIMAP_WIDTH 30  // Width of minimap in cells
# define MINIMAP_HEIGHT 30 // Height of minimap in cells
# define PLAYER_RADIUS 2 // Radius of the player on the minimap
# define CEILING_COLOR 0x87CEEB  // Light blue for the ceiling
# define FLOOR_COLOR 0x008000
# define MINIMAP_SCALE 5
//#define MINIMAP_SIZE 100

// COLORS
# define COLOR_GREEN		"\033[0;32m"	// len: 7
# define COLOR_RED		"\033[0;31m"	// len: 7
# define COLOR_YELLOW	"\033[0;33m"	// len: 7
# define COLOR_RESET		"\033[0m"		// len: 4
# define CR				"\033[0m"		// len: 4
# define RD				"\033[0;31m"	// len: 7

// KEYCODE
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65363
# define KEY_RIGHT 65361

typedef struct s_map
{
	int		fd; // close directly after done with the map file
	char	**map; // !! should be null terminated to free easily
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		f_color;
	int		c_color;
	char	**spl_ln; // free diretly after no using anymore // maybe better name
	char	*joined_lines;
	bool	id_ed;
	int		height;
	int		width;
}	t_map;

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bpp; //bits_per_pixel
	int		line_length;
	int		endian;
}	t_img;

typedef struct s_texture
{
	void	*img_ptr;
	char	*addr;
	int		width;
	int		height;
	int		bpp; //bits_per_pixel
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_render
{
	double	side_dist_x;
	double	side_dist_y;
	double	delta_x;
	double	delta_y;
	double	perp_dist_wall;
	double	wall_x;
	int		step_x;
	int		step_y;
	int		side;
	int		map_x;
	int		map_y;
	int		lineheight;
	int		draw_start;
	int		draw_end;
	int		tex_index;
}	t_render;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	ray_x;
	double	ray_y;
}	t_player;

typedef struct s_key_states
{
	int	left;
	int	right;
	int	w;
	int	s;
	int	a;
	int	d;
	int	esc;
}	t_key_states;

typedef struct s_minimap
{
	int	start_x;
	int	start_y;
	int	end_x;
	int	end_y;
	int	player_x;
	int	player_y;
}	t_minimap;

typedef struct s_main
{
	// mlx init and window
	void			*mlx_ptr;
	void			*win_ptr;
	t_img			mlx_img;
	t_map			u_map;
	t_player		*player;
	t_render		*render;
	t_texture		*west;
	t_texture		*east;
	t_texture		*north;
	t_texture		*south;
	int				*texture_buff[4];
	t_key_states	key_states;
}	t_main;

// cleanup
void	cleanup(t_main *cub, int stage);
void	free_texture_pointers(t_main *cub);
void	free_textures(t_map *u_map);
void	free_map(char **map);
void	free_map_elements(t_main *cub);

// map_val
int		map_val(t_main *cub, char *map_path);
int		handle_color(t_main *cub);
int		handle_texture(t_main *cub);
int		read_map_element(t_main *cub);
int		prep_for_init(char *line, t_main *cub);
int		read_check_txts_clrs(t_main *cub);

// map_val_pos
int		check_right(char *horiz, int c);
int		check_left(char *horiz, int c);
int		check_up(char **vert, int r, int c);
int		check_down(char **vert, int r, int c);
int		check_pos(char **map, int r, int c);

// game
int		game(t_main *cub);
void	get_ray_direction(t_player *player, int x);
void	get_draw_values(t_player *p, t_render *r);
void	get_distances(t_player *player, t_render *r);
void	hit_wall_loop(t_main *cub, t_render *render);
void	get_texture_index(t_player *player, t_render *r);

//player_init
void	init_player(t_player *player, t_map u_map);
void	init_player_pos(t_player *player, int x, int y);
void	init_w_e(t_player *player, int index);
void	init_s_n(t_player *player, int index);

//game_utils
int		render_background(t_main *cub);
int		close_window(t_main *cub);
void	pixel_put(t_img *mlx_img, int x, int y, int color);
int		check_right(char *horiz, int c);

// map_val_utils
int		str_ary_len(char **str_array);
void	free_str_array(char ***str_array);
bool	is_whitespace(char c);
bool	txts_clrs_found(t_map *u_map);
bool	is_val_ary_len(t_main *cub);
bool	is_cub_postfix(char *path);
int		check_texture_path(t_main *cub);
int		invalid_element(char *line);
int		get_max_len(char **map);
int		read_check_txts_clrs(t_main *cub);
int		prep_for_init(char *line, t_main *cub);
int		l_only_space(char **map);

//map_handle_color
int		init_u_map_clr(t_map *u_map);
int		comma_check(char *str);
int		inv_range(int *rgb);
char	*rm_zero(char *num);

//mlx_init
int		init_mlx(t_main *cub);

//player_movement
void	move_player(t_main *cub, double moveX, double moveY);
void	rotate_player(t_player *player, double angle);

//key_events
void	process_input(t_main *cub);
int		key_up(int keycode, t_main *cub);
int		key_down(int keycode, t_main *cub);

//minimap
void	draw_minimap(t_main *cub);

//textures
int		init_textures(t_main *cub);
int		load_textures(t_main *cub);
int		handle_textures(t_main *cub);
int		create_texture_buffer(t_main *cub);

// debuging
void	print_map_elements(t_map* u_map);

#endif
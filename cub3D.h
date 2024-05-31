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
#include <errno.h>
#include <mlx.h>

#include "libft/libft.h"
#include "libft/gnl/get_next_line.h"
#include "libft/printf/ft_printf.h"

typedef	struct	s_map
{
	char	**map;
	char	*no; //NO
	char	*so;
	char	*we;
	char	*ea;
	int		f_color;
	int		c_color;
}	t_map;



typedef struct	s_main
{
	// mlx init and window
	t_map	u_map;
}	t_main;

#endif
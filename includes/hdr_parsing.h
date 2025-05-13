/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdr_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:22:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 10:34:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef HDR_PARSING_H
# define HDR_PARSING_H

# include "libft.h"

typedef struct s_rgbe
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	e;
}	t_rgbe;

typedef struct s_hdr
{
	float	exposure;
	float	gamma;
	int		width;
	int		height;
	char	dir_x;
	char	dir_y;
	t_rgbe	*pixels;
}	t_hdr;

int	parse_hdr(t_hdr *hdr, char *filename);

int	print_err_hdr(char *str);

int	parse_hdr_header(t_hdr *hdr, t_bin *bin);
int	get_hdr_size(t_hdr *hdr, t_bin *bin, size_t index);
int	read_hdr_data(t_hdr *hdr, t_bin *bin, size_t index);


#endif

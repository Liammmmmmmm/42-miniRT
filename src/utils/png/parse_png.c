/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:42:51 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/11 18:01:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	print_err_png(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (-1);
}

int	get_png_file(t_bin *bin, t_sh_png_chunk *png)
{
	
}

int	parse_png(char *filename, t_tex_img *img)
{
	t_bin			bin;
	t_sh_png_chunk	png;

	if (!img)
		return (-1);
	if (!read_bin_file(&bin, filename))
		return (print_err_png("Error reading png file."));
	ft_bzero(&png, sizeof(t_sh_png_chunk));
	get_png_file(&bin, &png);
	free(bin.data);
	return (1);
}

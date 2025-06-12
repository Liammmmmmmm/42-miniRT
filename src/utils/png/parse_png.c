/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:42:51 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 15:24:06 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	print_err_png(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (-1);
}

int	get_png_file(t_bin *bin)
{
	t_png_info		png;
	size_t			i;

	i = 0;
	if (get_png_header(bin, &i) == -1)
		return (-1);
	if (read_first_png_chunk(bin, &i, &png) == -1)
		return (print_err_png(PNG_ERROR_GET_IHDR));
	print_ihdr_chunk(&png);
	return (0);
}

int	parse_png(char *filename, t_tex_img *img)
{
	t_bin	bin;

	if (!img)
		return (-1);
	if (!read_bin_file(&bin, filename))
		return (print_err_png("Error reading png file."));
	if (get_png_file(&bin) == -1)
		return (-1);
	free(bin.data);
	return (0);
}

int main()
{
	t_tex_img img;
	printf("Return global : %d\n", parse_png("test.png", &img));
}

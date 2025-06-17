/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:49:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 12:47:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "png_parser.h"

void	get_texture_image(t_tex *tex, char *filename)
{
	if (is_extension(filename, "png"))
	{
		printf("Parsing png texture `%s'\n", filename);
		if (parse_png(filename, &tex->img) == 0)
			printf("PNG file successfully parsed\n");
		else
			printf("PNG file parsing failed\n");
	}
	else if (is_extension(filename, "bmp"))
	{
		printf("Parsing bmp texture `%s'\n", filename);
		if (parse_bmp(filename, &tex->img) == 0)
			printf("BMP file successfully parsed\n");
		else
			printf("BMP file parsing failed\n");
	}
	else if (is_extension(filename, "hdr"))
	{
		printf("Parsing hdr texture `%s'\n", filename);
		if (parse_hdr(&tex->hdr, filename))
			printf("HDR file successfully parsed\n");
		else
			printf("HDR file parsing failed\n");
		tex->type = HDR;
	}
}

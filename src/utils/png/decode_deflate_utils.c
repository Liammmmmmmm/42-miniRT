/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_deflate_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:08:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 19:06:59 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	calculate_scanline_bytes(uint32_t width, uint8_t bit_depth,
	uint8_t color_type)
{
	int	channels;

	if (color_type == 0)
		channels = 1;
	else if (color_type == 2)
		channels = 3;
	else if (color_type == 3)
		channels = 1;
	else if (color_type == 4)
		channels = 2;
	else if (color_type == 6)
		channels = 4;
	else
		return (-1);
	return ((channels * bit_depth * width + 7) / 8);
}

int	get_bytes_per_pixel(uint8_t color_type, uint8_t bit_depth)
{
	if (bit_depth != 8)
		return (-1);
	if (color_type == 0)
		return (1);
	else if (color_type == 2)
		return (3);
	else if (color_type == 3)
		return (1);
	else if (color_type == 4)
		return (2);
	else if (color_type == 6)
		return (4);
	else
		return (-1);
}

static inline t_rgba	*err_extract(char *str)
{
	print_err_png(str);
	return (NULL);
}

static t_rgba	get_pixel_rgva(uint8_t *pixels, uint8_t bpp, uint32_t x)
{
	t_rgba	res;

	if (bpp == 1)
		res = (t_rgba){.r = pixels[x],
			.g = pixels[x], .b = pixels[x], .a = 255};
	else if (bpp == 2)
		res = (t_rgba){.r = pixels[x],
			.g = pixels[x], .b = pixels[x], .a = pixels[x * 2 + 1]};
	else
	{
		res = (t_rgba){.r = pixels[x * bpp + 0],
			.g = pixels[x * bpp + 1], .b = pixels[x * bpp + 2], .a = 255};
		if (bpp == 4)
			res.a = pixels[x * bpp + 3];
	}
	return (res);
}

t_rgba	*extract_rgba(uint8_t *image_data, t_png_info *infos,
	uint8_t bpp, uint32_t stride)
{
	t_rgba		*out;
	uint32_t	y;
	uint32_t	x;
	uint8_t		*pixels;

	if (bpp != 1 && bpp != 2 && bpp != 3 && bpp != 4)
		return (err_extract("Unsupported png bpp."));
	out = malloc(infos->width * infos->height * sizeof(t_rgba));
	if (!out)
		return (err_extract(PNG_ERROR_MEM));
	y = (uint32_t)(-1);
	while (++y < infos->height)
	{
		pixels = (image_data + (infos->height - y - 1) * stride) + 1;
		x = (uint32_t)(-1);
		while (++x < infos->width)
			out[y * infos->width + x] = get_pixel_rgva(pixels, bpp, x);
	}
	return (out);
}

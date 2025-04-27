/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_extract_pixel.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:50:32 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/27 18:41:58 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "bmp_parsing.h"

static inline int	allocate_pixel_data(t_bmp *bmp, size_t tpx)
{
	bmp->pixel_data = malloc(tpx * sizeof(t_color));
	if (bmp->pixel_data == NULL)
		return (error_and_return("error: unable to access memory\n"));
	return (0);
}

static inline int	read_pixels_for_row(t_bin *bin, size_t *i, t_bmp *bmp, \
	size_t y)
{
	size_t		x;
	uint32_t	color;

	x = 0;
	while (x < bmp->info.width)
	{
		if (extract_pixel(bin, i, bmp, &color) == -1)
			return (-1);
		bmp->pixel_data[y * bmp->info.width + x] = \
			(t_color){(color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF};
		x++;
	}
	return (0);
}

static inline int	skip_padding(t_bin *bin, size_t *i, size_t padding)
{
	size_t	p;
	uint8_t	tmp;

	p = 0;
	while (p < padding)
	{
		if (read_uint8_move(bin, i, &tmp) == 1)
			return (error_and_return("error: unable to read px\n"));
		p++;
	}
	return (0);
}

int	extract_raw_pixels(t_bin *bin, size_t *i, t_bmp *bmp)
{
	size_t			y;
	const size_t	tpx = bmp->info.width * bmp->info.height;
	size_t			padding;
	size_t			row_bytes;

	y = 0;
	row_bytes = ((bmp->info.width * bmp->info.bpp + 7) / 8);
	padding = (4 - (row_bytes % 4)) % 4;
	if (allocate_pixel_data(bmp, tpx) != 0)
		return (1);
	while (y < bmp->info.height)
	{
		if (read_pixels_for_row(bin, i, bmp, y) == -1)
		{
			free(bmp->pixel_data);
			return (1);
		}
		if (skip_padding(bin, i, padding) == 1)
		{
			free(bmp->pixel_data);
			return (1);
		}
		y++;
	}
	return (0);
}

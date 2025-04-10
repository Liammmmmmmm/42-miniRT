/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_extract_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:50:33 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/10 19:27:33 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "bmp_parsing.h"

int	extract_palette_pixel(t_bin *bin, size_t *i, uint32_t *value)
{
	uint8_t	palette_index;

	if (read_uint8_move(bin, i, &palette_index) == -1)
		return (1);
	*value = palette_index;
	return (0);
}

int	extract_16bpp_pixel(t_bin *bin, size_t *i, uint32_t *value)
{
	uint16_t	raw_pixel;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;

	raw_pixel = 0;
	if (read_uint16_move_little(bin, i, &raw_pixel) == -1)
		return (1);
	r = ((raw_pixel >> 11) & 0x1F) << 3;
	g = ((raw_pixel >> 5) & 0x3F) << 2;
	b = (raw_pixel & 0x1F) << 3;
	*value = ((r << 16) | (g << 8) | b);
	return (0);
}

int	extract_24bpp_pixel(t_bin *bin, size_t *i, uint32_t *value)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;

	if (read_uint8_move(bin, i, &b) == -1)
		return (1);
	if (read_uint8_move(bin, i, &g) == -1)
		return (1);
	if (read_uint8_move(bin, i, &r) == -1)
		return (1);
	*value = ((r << 16) | (g << 8) | b);
	return (0);
}

int	extract_32bpp_pixel(t_bin *bin, size_t *i, uint32_t *value)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint8_t		a;

	if (read_uint8_move(bin, i, &b) == -1)
		return (1);
	if (read_uint8_move(bin, i, &g) == -1)
		return (1);
	if (read_uint8_move(bin, i, &r) == -1)
		return (1);
	if (read_uint8_move(bin, i, &a) == -1)
		return (1);
	*value = ((a << 24) | (r << 16) | (g << 8) | b);
	return (0);
}

int	extract_pixel(t_bin *bin, size_t *i, t_bmp *bmp, uint32_t *value)
{
	uint32_t	pixel_value;

	pixel_value = 0;
	if (bmp->info.bpp == 1)
		return (extract_palette_pixel(bin, i, value));
	else if (bmp->info.bpp == 4)
		return (extract_palette_pixel(bin, i, value));
	else if (bmp->info.bpp == 8)
		return (extract_palette_pixel(bin, i, value));
	else if (bmp->info.bpp == 16)
		return (extract_16bpp_pixel(bin, i, value));
	else if (bmp->info.bpp == 24)
		return (extract_24bpp_pixel(bin, i, value));
	else
		return (extract_32bpp_pixel(bin, i, value));
	return (0);
}

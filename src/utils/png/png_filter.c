/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_filter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:35:22 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 09:39:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

void	apply_png_filter(uint8_t *image_data, t_png_info *infos,
	uint8_t bpp, uint32_t stride)
{
	uint32_t	y;
	uint8_t		*scanline;
	uint8_t		*prev;

	y = (uint32_t)(-1);
	while (++y < infos->height)
	{
		scanline = image_data + y * stride;
		prev = NULL;
		if (y != 0)
			prev = (scanline - stride + 1);
		if (scanline[0] == 0)
			continue ;
		else if (scanline[0] == 1)
			png_filter_sub(scanline + 1, infos->width * bpp, bpp);
		else if (scanline[0] == 2)
			png_filter_up(scanline + 1, prev, infos->width * bpp);
		else if (scanline[0] == 3)
			png_filter_average(scanline + 1, prev, infos->width * bpp, bpp);
		else if (scanline[0] == 4)
			png_filter_paeth(scanline + 1, prev, infos->width * bpp, bpp);
		else
			printf("Invalid png filter type: %d\n", scanline[0]);
	}
}

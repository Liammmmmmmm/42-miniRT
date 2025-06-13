/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:20:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 16:50:12 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

void	print_ihdr_chunk(t_png_info *png)
{
	printf("PNG IHDR informations:\n");
	printf("Width: %u, Height: %u\n", png->width, png->height);
	printf("depth      : %u\n", png->depth);
	printf("color type : %u\n", png->color_type);
	printf("compression: %u\n", png->compression);
	printf("filter     : %u\n", png->filter);
	printf("interlace  : %u\n", png->interlace);
}

void	debug_print_all_code_lengths(const uint8_t *all_code_lengths,
	size_t hlit, size_t hdist)
{
	size_t	i;

	printf("=== Code lengths for literals/lengths (hlit=%zu) ===\n", hlit);
	i = 0;
	while (i < hlit)
	{
		printf("Symbol %3zu: length = %u\n", i, all_code_lengths[i]);
		i++;
	}
	printf("=== Code lengths for distances (hdist=%zu) ===\n", hdist);
	i = 0;
	while (i < hdist)
	{
		printf("Distance %3zu: length = %u\n", i, all_code_lengths[hlit + i]);
		i++;
	}
}

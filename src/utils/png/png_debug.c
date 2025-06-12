/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:20:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 15:22:54 by lilefebv         ###   ########lyon.fr   */
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

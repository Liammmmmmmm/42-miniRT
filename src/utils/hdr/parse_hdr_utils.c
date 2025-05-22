/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:02:58 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/22 07:53:12 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"
#include <math.h>

size_t	get_line_bin(t_bin *bin, size_t current_index)
{
	size_t	tmp;

	current_index++;
	tmp = current_index;
	if (current_index >= bin->size)
		return ((size_t)(-1));
	while (tmp < bin->size && bin->data[tmp] && bin->data[tmp] != '\n')
		tmp++;
	if (bin->data[tmp] == '\n')
		bin->data[tmp] = '\0';
	return (current_index);
}

void	move_index_end(t_bin *bin, size_t *index)
{
	while (*index < bin->size && bin->data[*index])
		(*index)++;
}

float	average_luminance_hdr(t_hdr *hdr)
{
	float	scale;
	int		total;
	int		i;
	double	sum;

	sum = 0;
	total = hdr->width * hdr->height;
	i = 0;
	while (i < total)
	{
		scale = ldexp(1.0, hdr->pixels[i].e - 128) / 255;
		sum += 0.2126f * (hdr->pixels[i].r * scale)
			+ 0.7152f * (hdr->pixels[i].g * scale)
			+ 0.0722f * (hdr->pixels[i].b * scale);
		i++;
	}
	return (sum / total);
}

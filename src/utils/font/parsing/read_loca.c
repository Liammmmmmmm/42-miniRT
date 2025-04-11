/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_loca.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:06:45 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 13:15:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	read_loca_type(t_bin *bin, t_ttf *ttf)
{
	uint16_t	res;

	res = 0;
	read_uint16(bin, ttf->r_data.head_offset + 50, &res);
	return (res);
}

uint32_t	get_glyph_offset(t_bin *bin, t_ttf *ttf, uint32_t glyph_index)
{
	uint32_t	offset;
	uint16_t	off16;

	offset = 0;
	if (read_loca_type(bin, ttf))
		read_uint32(bin,
			ttf->r_data.loca_offset + glyph_index * sizeof(uint32_t), &offset);
	else
	{
		off16 = 0;
		read_uint16(bin,
			ttf->r_data.loca_offset + glyph_index * sizeof(uint16_t), &off16);
		offset = (uint32_t)off16 * 2;
	}
	return (offset);
}

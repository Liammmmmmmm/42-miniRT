/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_hmtx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:08:42 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 09:03:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	read_hmtx(t_bin *bin, t_ttf *ttf)
{
	int			i;
	size_t		lsb_offset;
	uint32_t	index;

	i = -1;
	while (++i < 256)
	{
		index = get_glyph_index(i, ttf->cmap.format4);
		if (index < ttf->hhea.num_hmetrics)
		{
			read_uint16(bin, ttf->r_data.hmtx_offset + index * 4,
				&ttf->glyph256[i].advance_width);
			read_int16(bin, ttf->r_data.hmtx_offset + index * 4 + 2,
				&ttf->glyph256[i].left_side_bearing);
		}
		else
		{
			read_uint16(bin, ttf->r_data.hmtx_offset + (ttf->hhea.num_hmetrics
					- 1) * 4, &ttf->glyph256[i].advance_width);
			lsb_offset = ttf->r_data.hmtx_offset + ttf->hhea.num_hmetrics * 4
				+ (i - ttf->hhea.num_hmetrics) * 2;
			read_int16(bin, lsb_offset, &ttf->glyph256[i].left_side_bearing);
		}
	}
	return (0);
}

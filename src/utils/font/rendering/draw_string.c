/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:42:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 18:22:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	draw_string(t_img *img, t_ttf *ttf, char *str, t_point2 pos)
{
	const t_point2	default_pt = pos;
	const float 	scale = ttf->size / (float)ttf->head.units_per_em;
	const int		strl = ft_sstrlen(str);
	int				i;

	i = -1;
	while (++i < strl)
	{
		if (str[i] == '\n')
		{
			pos.x = default_pt.x;
			pos.y = pos.y + (ttf->hhea.ascent - ttf->hhea.descent + ttf->hhea.line_gap) * scale;
			continue ;
		}
		else if (str[i] != ' ')
			draw_glyph_outline(img, ttf, str[i], pos);
		printf("Char : '%c' | advance : %d | lsb %d | i : %d\n", str[i], ttf->glyph256[(int)str[i]].advance_width, ttf->glyph256[(int)str[i]].left_side_bearing, get_glyph_index((uint16_t)str[i], ttf->cmap.format4));
		pos.x += ttf->glyph256[(int)str[i]].advance_width * scale;
	}
}

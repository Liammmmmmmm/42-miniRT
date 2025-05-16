/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:42:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 13:35:28 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	draw_string(t_img *img, t_ttf *ttf, const char *str, t_point2 pos)
{
	const t_point2	default_pt = pos;
	const float		scale = ttf->size / (float)ttf->head.units_per_em;
	const int		strl = ft_sstrlen(str);
	int				i;

	if (!str)
		return ;
	i = -1;
	while (++i < strl)
	{
		if (str[i] == '\n')
		{
			pos.x = default_pt.x;
			pos.y += (ttf->hhea.ascent - ttf->hhea.descent + ttf->hhea.line_gap)
				* scale;
			continue ;
		}
		else if (str[i] != ' ')
			draw_glyph(img, ttf, str[i], pos);
		pos.x += ttf->glyph256[(uint8_t)str[i]].advance_width * scale;
	}
}

uint32_t	get_string_width(t_ttf *ttf, char *str)
{
	const float		scale = ttf->size / (float)ttf->head.units_per_em;
	const int		strl = ft_sstrlen(str);
	int				i;
	uint32_t		res[2];

	i = -1;
	res[0] = 0;
	res[1] = 1;
	while (++i < strl)
	{
		if (str[i] == '\n')
		{
			if (res[0] > res[1])
				res[1] = res[0];
			res[0] = 0;
			continue ;
		}
		res[0] += ttf->glyph256[(uint8_t)str[i]].advance_width * scale;
	}
	if (res[0] > res[1])
		return (res[0]);
	return (res[1]);
}

uint32_t	get_height(t_ttf *ttf)
{
	return ((ttf->hhea.ascent + ttf->hhea.descent)
		* (ttf->size / (float)ttf->head.units_per_em));
}

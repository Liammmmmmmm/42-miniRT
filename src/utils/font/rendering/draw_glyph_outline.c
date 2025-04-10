/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_glyph_outline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:41:04 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 11:41:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	draw_glyph_outline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	uint32_t	i;
	t_point		p1;
	t_point		p2;
	const float	scale = ttf->size / (float)ttf->head.units_per_em;

	i = 0;
	if (!ttf->glyph256[c].segments)
		return ;
	while (i < ttf->glyph256[c].segments_amount)
	{
		
		p1.x = ttf->glyph256[c].segments[i].p1.x * scale + pos.x;
		p1.y = ttf->glyph256[c].segments[i].p1.y * -scale + pos.y;
		p2.x = ttf->glyph256[c].segments[i].p2.x * scale + pos.x;
		p2.y = ttf->glyph256[c].segments[i].p2.y * -scale + pos.y;
		draw_line(&p1, &p2, img);
		i++;
	}
}

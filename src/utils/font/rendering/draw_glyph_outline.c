/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_glyph_outline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:41:04 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 09:39:14 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	draw_glyph_outline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	int i = 0;
	const float scale = ttf->size / (float)ttf->head.units_per_em;
	const int lsb = ttf->glyph256[c].left_side_bearing - ttf->glyph256[c].xmin;

	//print_glyph_outline(&ttf->glyph256[c]);
	while (i < ttf->glyph256[c].bezier_amount)
	{
		if (ttf->glyph256[c].bezier_lines[i].have_control == 0)
		{
			t_point p1;
			p1.x = (ttf->glyph256[c].bezier_lines[i].p1.x + lsb) * scale + pos.x;
			p1.y = ttf->glyph256[c].bezier_lines[i].p1.y * -scale + pos.y;
			t_point p2;
			p2.x = (ttf->glyph256[c].bezier_lines[i].p2.x + lsb) * scale + pos.x;
			p2.y = ttf->glyph256[c].bezier_lines[i].p2.y * -scale + pos.y;
			draw_line(&p1, &p2, img);
		}
		else
		{
			uint32_t size = 10;
			t_point2 points[11];
			uint32_t y = 0;
			tessellate_bezier(points, &size, &ttf->glyph256[c].bezier_lines[i]);
			while (y < size)
			{
				t_point p1;
				p1.x = (points[y].x + lsb) * scale + pos.x;
				p1.y = points[y].y * -scale + pos.y;
				t_point p2;
				p2.x = (points[y + 1].x + lsb) * scale + pos.x;
				p2.y = points[y + 1].y * -scale + pos.y;
				draw_line(&p1, &p2, img);
				y++;
			}
		}
		i++;
	}
}

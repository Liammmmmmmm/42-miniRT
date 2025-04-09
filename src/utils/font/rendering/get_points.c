/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_points.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:53:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 10:04:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	draw_glyph(t_img *img, t_ttf *ttf, t_uchar c)
{
	int i = 0;

	while (i < ttf->glyph256[c].bezier_amount)
	{
		if (ttf->glyph256[c].bezier_lines[i].have_control == 0)
		{
			t_point p1;
			p1.x = ttf->glyph256[c].bezier_lines[i].p1.x;
			p1.y = ttf->glyph256[c].bezier_lines[i].p1.y;
			t_point p2;
			p2.x = ttf->glyph256[c].bezier_lines[i].p2.x;
			p2.y = ttf->glyph256[c].bezier_lines[i].p2.y;
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
				p1.x = points[y].x;
				p1.y = points[y].y;
				t_point p2;
				p2.x = points[y + 1].x;
				p2.y = points[y + 1].y;
				draw_line(&p1, &p2, img);
				y++;
			}
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_points.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:53:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 15:24:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	draw_glyph(t_img *img)
{
	t_ttf	ttf;

	get_font(&ttf, "test3.ttf");


	int i = 0;
	while (i < ttf.outline.bezier_amount)
	{
		if (ttf.outline.bezier_lines[i].have_control == 0)
		{
			t_point p1;
			p1.x = ttf.outline.bezier_lines[i].p1.x;
			p1.y = ttf.outline.bezier_lines[i].p1.y;
			t_point p2;
			p2.x = ttf.outline.bezier_lines[i].p2.x;
			p2.y = ttf.outline.bezier_lines[i].p2.y;
			draw_line(&p1, &p2, img);
		}
		else
		{
			uint32_t size = 10;
			t_point2 points[size];
			uint32_t y = 0;
			printf("aaaa p1 : %d %d | p2 : %d %d | pc : %d %d\n", ttf.outline.bezier_lines[i].p1.x, ttf.outline.bezier_lines[i].p1.y, ttf.outline.bezier_lines[i].p2.x, ttf.outline.bezier_lines[i].p2.y,ttf.outline.bezier_lines[i].pc.x, ttf.outline.bezier_lines[i].pc.y);
			tessellate_bezier(points, &size, &ttf.outline.bezier_lines[i]);
			while (y < size - 1)
			{
				t_point p1;
				p1.x = points[y].x;
				p1.y = points[y].y;
				t_point p2;
				p2.x = points[y + 1].x;
				p2.y = points[y + 1].y;
				printf("p1 : %d %d | p2 : %d %d\n", p1.x, p1.y, p2.x, p2.y);
				draw_line(&p1, &p2, img);
				y++;
			}
		}
		
		printf("JDRAW LA LINE\n");
		i++;
	}
}

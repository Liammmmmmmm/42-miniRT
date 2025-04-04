/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_points.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:53:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/04 14:04:00 by lilefebv         ###   ########lyon.fr   */
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
		t_point p1;
		p1.x = ttf.outline.bezier_lines[i].p1.x;
		p1.y = ttf.outline.bezier_lines[i].p1.y;
		t_point p2;
		p2.x = ttf.outline.bezier_lines[i].p2.x;
		p2.y = ttf.outline.bezier_lines[i].p2.y;
		draw_line(&p1, &p2, img);
		printf("JDRAW LA LINE\n");
		i++;
	}
}

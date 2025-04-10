/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:44:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 16:34:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static void	min_max_seg(t_point2 *p1, t_point2 *p2, t_segment seg)
{
	if (seg.p1.y < seg.p2.y)
	{
		p1->x = seg.p1.x;
		p1->y = seg.p1.y;
		p2->x = seg.p2.x;
		p2->y = seg.p2.y;
	}
	else
	{
		p1->x = seg.p2.x;
		p1->y = seg.p2.y;
		p2->x = seg.p1.x;
		p2->y = seg.p1.y;
	}
}

static uint16_t	get_all_seg_intersect(t_ttf *ttf, t_glyph_outline *o, int y,
	const float scale)
{
	t_uint		i;
	uint16_t	n;
	t_point2	p1;
	t_point2	p2;

	i = -1;
	n = 0;
	while (++i < o->segments_amount
		&& n < ttf->r_data.estimated_max_seg_intersect)
	{
		min_max_seg(&p1, &p2, o->segments[i]);
		if (y <= p1.y || y > p2.y)
			continue ;
		ttf->r_data.seg_intersec[n] = \
		(p1.x + ((y - p1.y) / (float)(p2.y - p1.y)) * (p2.x - p1.x)) * scale;
		n++;
	}
	return (n);
}

static uint16_t	count_intersect_pos(int32_t *interlist, uint16_t num_seg, int x)
{
	uint16_t	i;
	uint16_t	res;

	i = 0;
	res = 0;
	while (i < num_seg)
	{
		if (x == interlist[i])
			res++;
		i++;
	}
	return (res);
}

static void	draw_scanline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	const uint16_t	num_seg = get_all_seg_intersect(ttf, &ttf->glyph256[c],
			ttf->r_data.ytmp / ttf->r_data.scale, ttf->r_data.scale);
	int32_t			x;
	uint16_t		intersects;

	x = ttf->glyph256[c].xmin * ttf->r_data.scale;
	intersects = 0;
	while (x <= ttf->r_data.xmax)
	{
		intersects += count_intersect_pos(ttf->r_data.seg_intersec, num_seg, x);
		if (intersects % 2 == 1)
			put_pixel_image(img, pos.x + x, pos.y - ttf->r_data.ytmp,
				ttf->color);
		x++;
	}
}

void	draw_glyph(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	int	maxy;
	int	y;

	ttf->r_data.scale = ttf->size / (float)ttf->head.units_per_em;
	ttf->r_data.xmax = ttf->glyph256[c].xmax * ttf->r_data.scale;
	maxy = ttf->glyph256[c].ymax * ttf->r_data.scale;
	y = ttf->glyph256[c].ymin * ttf->r_data.scale;
	while (y < maxy)
	{
		ttf->r_data.ytmp = y;
		draw_scanline(img, ttf, c, pos);
		y++;
	}
}

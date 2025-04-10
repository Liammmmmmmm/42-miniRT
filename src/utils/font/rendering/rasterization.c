/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:44:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 15:28:13 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	min_max_seg(t_point2 *p1, t_point2 *p2, t_segment seg, const float scale)
{
	if (seg.p1.y < seg.p2.y)
	{
		p1->x = seg.p1.x * scale;
		p1->y = seg.p1.y * scale;
		p2->x = seg.p2.x * scale;
		p2->y = seg.p2.y * scale;
	}
	else
	{
		p1->x = seg.p2.x * scale;
		p1->y = seg.p2.y * scale;
		p2->x = seg.p1.x * scale;
		p2->y = seg.p1.y * scale;
	}
}

int	segment_intersects_scanline(t_segment seg, int y, const float scale)
{
	t_point2	p1;
	t_point2	p2;

	min_max_seg(&p1, &p2, seg, scale);
	if (y <= p1.y || y > p2.y)
		return (-1);
	return (p1.x + ((y - p1.y) / (float)(p2.y - p1.y)) * (p2.x - p1.x));
}

uint16_t	get_all_seg_intersect(t_ttf *ttf, t_glyph_outline *o, int y, const float scale)
{
	t_uint		i;
	uint16_t	n;

	i = -1;
	n = 0;
	while (++i < o->segments_amount && n < ttf->r_data.estimated_max_seg_intersect)
	{
		ttf->r_data.seg_intersec[n] = segment_intersects_scanline(o->segments[i], y, scale);
		if (ttf->r_data.seg_intersec[n] == -1)
			continue ;
		n++;
	}
	return (n);
}

uint16_t	count_intersect_at_pos(int32_t *interlist, uint16_t num_seg, int x)
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

void	draw_scanline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	const uint16_t	num_seg = get_all_seg_intersect(ttf, &ttf->glyph256[c], ttf->r_data.ytmp, ttf->r_data.scale);
	int32_t			x;
	uint16_t		intersects;

	x = 0;
	intersects = 0;
	while (x < ttf->r_data.xmax)
	{
		intersects += count_intersect_at_pos(ttf->r_data.seg_intersec, num_seg, x);
		if (intersects % 2 == 1)
			put_pixel_image(img, pos.x + x, pos.y - ttf->r_data.ytmp, 0xFFFFFF);
		x++;
	}
}

void	draw_glyph(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	int	maxy;
	int	y;

	printf("Start glyph\n");

	ttf->r_data.scale = ttf->size / (float)ttf->head.units_per_em;
	ttf->r_data.xmax = ttf->glyph256[c].xmax * ttf->r_data.scale;
	maxy = (ttf->glyph256[c].ymax - ttf->glyph256[c].ymin) * ttf->r_data.scale;

	y = ttf->glyph256[c].ymin * ttf->r_data.scale;
	while (y < maxy)
	{
		ttf->r_data.ytmp = y;
		draw_scanline(img, ttf, c, pos);
		y++;
	}
}

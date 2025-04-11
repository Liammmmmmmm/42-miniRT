/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:44:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 11:46:47 by lilefebv         ###   ########lyon.fr   */
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

static uint16_t	*get_all_seg_intersect(t_ttf *ttf, t_glyph_outline *o, int y,
	uint16_t n[ANTI_ALIASING_LEVEL])
{
	t_uint		i;
	int			m;
	t_point2	p1;
	t_point2	p2;

	m = -1;
	while (++m < ANTI_ALIASING_LEVEL)
	{
		i = -1;
		n[m] = 0;
		while (++i < o->segments_amount
			&& n[m] < ttf->r_data.estimated_max_seg_intersect)
		{
			min_max_seg(&p1, &p2, o->segments[i]);
			if (y <= p1.y || y > p2.y)
				continue ;
			ttf->r_data.seg_intersec[m][n[m]] = (p1.x + ((y - p1.y) / \
			(float)(p2.y - p1.y)) * (p2.x - p1.x)) * ttf->r_data.scale;
			n[m]++;
		}
		y++;
	}
	return (n);
}

static uint16_t	count_intersect_pos(
	int32_t interlist[ANTI_ALIASING_LEVEL][ESTIMATED_MAX_INTERSECT],
	uint16_t num_seg[ANTI_ALIASING_LEVEL],
	int x,
	uint16_t intersects[ANTI_ALIASING_LEVEL])
{
	uint16_t	i;
	uint16_t	res;
	int32_t		y;

	y = -1;
	res = 0;
	while (++y < ANTI_ALIASING_LEVEL)
	{
		i = 0;
		while (i < num_seg[y])
		{
			if (x == interlist[y][i])
				intersects[y]++;
			i++;
		}
		if (intersects[y] % 2 == 1)
			res++;
	}
	return (res);
}

static void	draw_scanline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	uint16_t	ns[ANTI_ALIASING_LEVEL];
	int32_t		x;
	uint16_t	in[ANTI_ALIASING_LEVEL];
	uint16_t	total_inter;

	get_all_seg_intersect(ttf, &ttf->glyph256[c],
		ttf->r_data.ytmp / ttf->r_data.scale, ns);
	x = ttf->glyph256[c].xmin * ttf->r_data.scale;
	ttf->r_data.xstart = x;
	ft_bzero(in, sizeof(uint16_t) * ANTI_ALIASING_LEVEL);
	while (x <= ttf->r_data.xmax)
	{
		if ((x - ttf->r_data.xstart) % ANTI_ALIASING_LEVEL == 0)
			total_inter = 0;
		total_inter += count_intersect_pos(ttf->r_data.seg_intersec, ns, x, in);
		if ((x - ttf->r_data.xstart) % ANTI_ALIASING_LEVEL
			== ANTI_ALIASING_LEVEL - 1)
			put_pixel_image(img, pos.x + (x / ANTI_ALIASING_LEVEL), pos.y \
	- (ttf->r_data.ytmp / ANTI_ALIASING_LEVEL), \
calc_gradiant_color(0, ttf->color, (float)total_inter / ttf->r_data.aalvlsqr));
		x++;
	}
}

void	draw_glyph(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos)
{
	int	maxy;
	int	y;

	ttf->r_data.scale = ttf->size / (float)ttf->head.units_per_em
		* ANTI_ALIASING_LEVEL;
	ttf->r_data.xmax = ttf->glyph256[c].xmax * ttf->r_data.scale;
	maxy = ttf->glyph256[c].ymax * ttf->r_data.scale;
	y = ttf->glyph256[c].ymin * ttf->r_data.scale;
	while (y <= maxy)
	{
		ttf->r_data.ytmp = y;
		draw_scanline(img, ttf, c, pos);
		y += ANTI_ALIASING_LEVEL;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/26 20:17:10 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minirt.h"

// static inline t_color	bilerp(t_color c00, t_color c10, t_color c01, t_color c11, float tx, float ty)
// {
// 	return ((t_color){
// 		(c00.r + (c10.r - c00.r) * tx) + ((c01.r + (c11.r - c01.r) * tx) - (c00.r + (c10.r - c00.r) * tx)) * ty,
// 		(c00.g + (c10.g - c00.g) * tx) + ((c01.g + (c11.g - c01.g) * tx) - (c00.g + (c10.g - c00.g) * tx)) * ty,
// 		(c00.b + (c10.b - c00.b) * tx) + ((c01.b + (c11.b - c01.b) * tx) - (c00.b + (c10.b - c00.b) * tx)) * ty
// 	});
// }

static inline void	process_pixel(t_upscale_data *d, const int i, const int px, const int py)
{
	register int				x1;
	register int				y1;
	const register float		tx = px * d->scaleX - d->x;
	const register float		ty = py * d->scaleY - d->y;
	register t_color			c[4];
	t_sc_point					p;

	if (d->x + 1 < d->sw)
		x1 = d->x + 1;
	else
		x1 = d->x;
	if (d->y + 1 < d->sh)
		y1 = d->y + 1;
	else
		y1 = d->y;	
	c[0] = (t_color){d->minirt->screen.render[d->y * d->sw + d->x].color.r / \
		d->divide, d->minirt->screen.render[d->y * d->sw + d->x].color.g / d->divide,
		d->minirt->screen.render[d->y * d->sw + d->x].color.b / d->divide};
	c[1] = (t_color){d->minirt->screen.render[d->y * d->sw + x1].color.r / \
		d->divide,d->minirt->screen.render[d->y * d->sw + x1].color.g / d->divide,
		d->minirt->screen.render[d->y * d->sw + x1].color.b / d->divide};
	c[2] = (t_color){d->minirt->screen.render[y1 * d->sw + d->x].color.r / \
		d->divide,d->minirt->screen.render[y1 * d->sw + d->x].color.g / d->divide,
		d->minirt->screen.render[y1 * d->sw + d->x].color.b / d->divide};
	c[3] = (t_color){d->minirt->screen.render[y1 * d->sw + x1].color.r / \
		d->divide,d->minirt->screen.render[y1 * d->sw + x1].color.g / d->divide,
		d->minirt->screen.render[y1 * d->sw + x1].color.b / d->divide};
	p.x = d->minirt->screen.render[i].x;
	p.y = d->minirt->screen.render[i].y;
	p.z = 0;
	p.color = ((t_color){
		(c[0].r + (c[1].r - c[0].r) * tx) + ((c[2].r + (c[3].r - c[2].r) * tx) - (c[0].r + (c[1].r - c[0].r) * tx)) * ty,
		(c[0].g + (c[1].g - c[0].g) * tx) + ((c[2].g + (c[3].g - c[2].g) * tx) - (c[0].g + (c[1].g - c[0].g) * tx)) * ty,
		(c[0].b + (c[1].b - c[0].b) * tx) + ((c[2].b + (c[3].b - c[2].b) * tx) - (c[0].b + (c[1].b - c[0].b) * tx)) * ty
	});
	put_sp_image(&d->minirt->mlx.img, &p);
}

void	bilinear_upscale(t_minirt *minirt)
{
	t_upscale_data				d;
	register int				i;
	register int				px;
	register int				py;

	d.minirt = minirt;
	d.sw = minirt->viewport.render_w;
	d.sh = minirt->viewport.render_h;
	d.dw = minirt->mlx.img.width;
	d.dh = minirt->mlx.img.height;
	d.divide = minirt->screen.last_sample_am;
	if (d.divide == 0)
		d.divide = 1;
	d.scaleX = (float)(d.sw - 1) / (d.dw - 1);
	d.scaleY = (float)(d.sh - 1) / (d.dh - 1);
	i = 0;
	py = 0;
	while (py < d.dh)
	{
		px = 0;
		while (px < d.dw)
		{
			d.x = (int)(px * d.scaleX);
			d.y = (int)(py * d.scaleY);
			process_pixel(&d, i, px, py);
			px++;
			i++;
		}
		py++;
	}
}

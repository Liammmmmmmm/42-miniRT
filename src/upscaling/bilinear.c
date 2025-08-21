/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 19:05:16 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minirt.h"

static inline void	set_xy(t_upscale_data *d, int *x, int *y)
{
	if (d->x + 1 < d->sw)
		*x = d->x + 1;
	else
		*x = d->x;
	if (d->y + 1 < d->sh)
		*y = d->y + 1;
	else
		*y = d->y;
}

static inline void	process_pixel(t_upscale_data *d, const int px, const int py)
{
	int				x1;
	int				y1;
	const float		tx = px * d->scale_x - d->x;
	const float		ty = py * d->scale_y - d->y;
	t_color			c[4];

	set_xy(d, &x1, &y1);
	c[0] = render_float_to_color(
			&d->minirt->screen.float_render[d->y * d->sw + d->x], d->divide);
	c[1] = render_float_to_color(
			&d->minirt->screen.float_render[d->y * d->sw + x1], d->divide);
	c[2] = render_float_to_color(
			&d->minirt->screen.float_render[y1 * d->sw + d->x], d->divide);
	c[3] = render_float_to_color(
			&d->minirt->screen.float_render[y1 * d->sw + x1], d->divide);
	d->minirt->screen.render[py * d->minirt->scene.win_width + px] = \
	color_to_int((t_color){(c[0].r + (c[1].r - c[0].r) * tx) + ((c[2].r + \
	(c[3].r - c[2].r) * tx) - (c[0].r + (c[1].r - c[0].r) * tx)) * ty, \
		(c[0].g + (c[1].g - c[0].g) * tx) + ((c[2].g + (c[3].g - c[2].g) * tx) \
		- (c[0].g + (c[1].g - c[0].g) * tx)) * ty, \
		(c[0].b + (c[1].b - c[0].b) * tx) + ((c[2].b + (c[3].b - c[2].b) * tx) \
		- (c[0].b + (c[1].b - c[0].b) * tx)) * ty});
}

void	bilinear_upscale(t_minirt *minirt)
{
	t_upscale_data	d;
	int				px;
	int				py;

	init_upscale_struct(&d, minirt);
	py = -1;
	while (++py < d.dh)
	{
		px = -1;
		while (++px < d.dw)
		{
			d.x = (int)(px * d.scale_x);
			d.y = (int)(py * d.scale_y);
			process_pixel(&d, px, py);
		}
	}
	copy_buff_to_image(minirt);
}

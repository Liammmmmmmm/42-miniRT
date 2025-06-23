/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicubic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:54:36 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 13:03:11 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static inline int	mirror_edge(int pos, int max)
{
	if (pos < 0)
		return (-pos - 1);
	if (pos >= max)
		return (2 * max - pos - 1);
	return (pos);
}

static inline float	cubic_weight(float t, int n)
{
	if (n == 0)
		return ((t * (t - 1) * (t - 2)) / -6.0f);
	if (n == 1)
		return (((t + 1) * (t - 1) * (t - 2)) / 2.0f);
	if (n == 2)
		return (((t + 1) * t * (t - 2)) / -2.0f);
	return (((t + 1) * t * (t - 1)) / 6.0f);
}

static inline void	calc_bicubic_v_val(t_upscale_data *d,
	t_interpolate_pixel_bicubic *ipb)
{
	int		j;
	int		i;
	float	weight;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			weight = ipb->wx[j] * ipb->wy[i];
			ipb->v[0] += (int)(clamp_double(d->minirt->screen.float_render
					[ipb->y[i] * d->sw + ipb->x[j]].r / d->divide) * 255)
				*weight;
			ipb->v[1] += (int)(clamp_double(d->minirt->screen.float_render
					[ipb->y[i] * d->sw + ipb->x[j]].g / d->divide) * 255)
				*weight;
			ipb->v[2] += (int)(clamp_double(d->minirt->screen.float_render
					[ipb->y[i] * d->sw + ipb->x[j]].b / d->divide) * 255)
				*weight;
			ipb->v[3] += weight;
		}
	}
}

static inline int	interpolate_pixel(t_upscale_data *d, int pos[2], float t[2])
{
	t_interpolate_pixel_bicubic	ipb;
	int							i;

	ft_bzero(&ipb, sizeof(t_interpolate_pixel_bicubic));
	i = -1;
	while (++i < 4)
	{
		ipb.x[i] = mirror_edge(pos[0] + i, d->sw);
		ipb.y[i] = mirror_edge(pos[1] + i, d->sh);
		ipb.wx[i] = cubic_weight(t[0], i);
		ipb.wy[i] = cubic_weight(t[1], i);
	}
	calc_bicubic_v_val(d, &ipb);
	return (color_to_int((t_color){clampf(ipb.v[0] / ipb.v[3], 0.f, 255.f),
			clampf(ipb.v[1] / ipb.v[3], 0.f, 255.f),
			clampf(ipb.v[2] / ipb.v[3], 0.f, 255.f)}));
}

void	bicubic_upscale(t_minirt *minirt)
{
	t_upscale_data	d;
	int				xy[2];
	float			t[2];
	float			src;
	int				y0;

	xy[1] = -1;
	init_upscale_struct(&d, minirt);
	while (++xy[1] < d.dh)
	{
		src = xy[1] * d.scaleY;
		t[1] = src - (int)src;
		y0 = (int)src - 1;
		xy[0] = -1;
		while (++xy[0] < d.dw)
		{
			src = xy[0] * d.scaleX;
			t[0] = src - (int)src;
			minirt->screen.render[xy[1] * d.dw + xy[0]]
				= interpolate_pixel(&d, (int [2]){(int)src - 1, y0}, t);
		}
	}
	copy_buff_to_image(minirt);
}

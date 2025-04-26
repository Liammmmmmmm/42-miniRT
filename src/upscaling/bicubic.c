/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicubic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:54:36 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/26 19:56:54 by madelvin         ###   ########.fr       */
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

static inline float clampf(float x, float min, float max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

static inline t_color	get_color(t_minirt *minirt, int i, int divide)
{
	return ((t_color){minirt->screen.render[i].color.r / divide, minirt->screen.render[i].color.g / divide, minirt->screen.render[i].color.b / divide});
}

static inline void	interpolate_pixel(t_minirt *minirt, int sw, int sh,
	int pos[2], float t[2], t_color *out, int divide)
{
	float	v[4] = {0, 0, 0, 0};
	float	wx[4], wy[4];
	int		x[4], y[4];

	for (int i = 0; i < 4; i++)
	{
		x[i] = mirror_edge(pos[0] + i, sw);
		y[i] = mirror_edge(pos[1] + i, sh);
		wx[i] = cubic_weight(t[0], i);
		wy[i] = cubic_weight(t[1], i);
	}
	for (int m = 0; m < 4; m++)
	{
		for (int n = 0; n < 4; n++)
		{
			t_color pix = get_color(minirt, y[m] * sw + x[n], divide);
			float weight = wx[n] * wy[m];
			v[0] += pix.r * weight;
			v[1] += pix.g * weight;
			v[2] += pix.b * weight;
			v[3] += weight;
		}
	}
	float inv_w = 1.0f / v[3];
	out->r = clampf(v[0] * inv_w, 0.f, 255.f);
	out->g = clampf(v[1] * inv_w, 0.f, 255.f);
	out->b = clampf(v[2] * inv_w, 0.f, 255.f);
}

void	bicubic_upscale(t_minirt *minirt)
{
	t_upscale_data	d;
	int				xy[2];
	float			t[2];
	int				dst_index;
	float			src_y;
	t_sc_point		point;

	point.z = 0;
	xy[1] = 0;
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
	while (xy[1] < d.dh)
	{
		src_y = xy[1] * d.scaleY;
		t[1] = src_y - (int)src_y;
		int y0 = (int)src_y - 1;
		xy[0] = 0;
		while (xy[0] < d.dw)
		{
			float src_x = xy[0] * d.scaleX;
			t[0] = src_x - (int)src_x;
			int x0 = (int)src_x - 1;
			int pos[2] = { x0, y0 };
			dst_index = xy[1] * d.dw + xy[0];
			point.x = minirt->screen.render[dst_index].x;
			point.y = minirt->screen.render[dst_index].y;
			interpolate_pixel(minirt, d.sw, d.sh, pos, t, &point.color, d.divide);
			put_sp_image(&minirt->mlx.img, &point);
			xy[0]++;
		}
		xy[1]++;
	}
}

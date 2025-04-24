/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:31:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 11:34:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

t_color	int_to_color(int color)
{
	return ((t_color){(color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF});
}

int	color_to_int(t_color color)
{
	return ((int)((color.r << 16) + (color.g << 8) + color.b));
}

int hsv_to_rgb(int h, float s, float v)
{
	const float c = v * s;
	const float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
	const float m = v - c;
	t_color		color;

	if (h < 60)
		color = (t_color){.r = (c + m) * 255, .g = (x + m) * 255, .b = m * 255};
	else if (h < 120)
		color = (t_color){.r = (x + m) * 255, .g = (c + m) * 255, .b = m * 255};
	else if (h < 180)
		color = (t_color){.r = m * 255, .g = (c + m) * 255, .b = (x + m) * 255};
	else if (h < 240)
		color = (t_color){.r = m * 255, .g = (x + m) * 255, .b = (c + m) * 255};
	else if (h < 300)
		color = (t_color){.r = (x + m) * 255, .g = m * 255, .b = (c + m) * 255};
	else
		color = (t_color){.r = (c + m) * 255, .g = m * 255, .b = (x + m) * 255};
	return ((int)((color.r << 16) + (color.g << 8) + color.b));
}

t_hsv_color rgb_to_hsv(int rgb)
{
	const t_fcolor	c = (t_fcolor){.r = ((rgb >> 16) & 0xFF) / 255.0,
		.g = ((rgb >> 8) & 0xFF) / 255.0, .b = (rgb & 0xFF) / 255.0};
	const float		max = fmaxf(c.r, fmaxf(c.g, c.b));
	const float		min = fminf(c.r, fminf(c.g, c.b));
	const float		delta = max - min;
	t_hsv_color		res;

	if (delta == 0)
		res.hue = 0;
	else if (max == c.r)
		res.hue = (int)(60 * fmodf((c.g - c.b) / delta, 6));
	else if (max == c.g)
		res.hue = (int)(60 * ((c.b - c.r) / delta + 2));
	else
		res.hue = (int)(60 * ((c.r - c.g) / delta + 4));
	if (res.hue < 0)
		res.hue += 360;
	res.sat = (max == 0) ? 0 : (delta / max);
	res.val = max;
	return (res);
}


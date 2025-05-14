/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:37:08 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/14 12:40:12 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double		clamp_double_val(double v, double min, double max)
{
	if (v <= min)
		return (min);
	else if (v >= max)
		return (max);
	return (v);
}

t_fcolor	clamp_fcolor_val(t_fcolor color, double min, double max)
{
	return ((t_fcolor){
		.r = clamp_double_val(color.r, min, max),
		.g = clamp_double_val(color.g, min, max),
		.b = clamp_double_val(color.b, min, max)
	});
}

t_fcolor	get_background_color(t_minirt *minirt, t_ray ray)
{
	int		i;
	double	u;
	double	v;

	if (minirt->scene.amb_light.skybox_t == NULL)
		return (color_to_fcolor(minirt->scene.amb_light.skybox_c));
	else
	{
		ray.dir = vec3_unit(ray.dir);
		v = 0.5 + asin(ray.dir.y) / PI_D;
		u = 0.5 - atan2(ray.dir.z, ray.dir.x) / (2.0 * PI_D);
		if (minirt->scene.amb_light.skybox_t->type == IMAGE && minirt->scene.amb_light.skybox_t->img.pixel_data)
		{
			i = minirt->scene.amb_light.skybox_t->img.width
				* (int)(v * minirt->scene.amb_light.skybox_t->img.height)
				+ (int)(u * minirt->scene.amb_light.skybox_t->img.width);
			return (color_to_fcolor(minirt->scene.amb_light.skybox_t->img.pixel_data[i]));
		}
		else if (minirt->scene.amb_light.skybox_t->type == HDR && minirt->scene.amb_light.skybox_t->hdr.pixels)
			return (clamp_fcolor_val(get_hdr_pixel(&minirt->scene.amb_light.skybox_t->hdr, u * (minirt->scene.amb_light.skybox_t->hdr.width - 1), v * (minirt->scene.amb_light.skybox_t->hdr.height - 1)), 0.0, 16.0));
		else
		{
			return ((t_fcolor){0.0, 0.0, 0.0});
		}
	}
}

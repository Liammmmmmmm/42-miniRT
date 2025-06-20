/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:37:08 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 16:40:13 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_fcolor	get_hdr_color(t_minirt *minirt, t_ray ray, char clamp)
{
	double	u;
	double	v;

	v = 0.5 + asin(ray.dir.y) / PI_D;
	u = 0.5 - atan2(ray.dir.z, ray.dir.x) / (2.0 * PI_D);
	if (minirt->scene.amb_light.skybox_t->type == HDR
		&& minirt->scene.amb_light.skybox_t->hdr.pixels)
	{
		if (clamp)
			return (clamp_fcolor_val(get_hdr_pixel_skybox(minirt,
						&minirt->scene.amb_light.skybox_t->hdr, u
						* (minirt->scene.amb_light.skybox_t->hdr.width - 1), v \
				* (minirt->scene.amb_light.skybox_t->hdr.height - 1)), 0, 16));
		return (get_hdr_pixel_skybox(minirt,
				&minirt->scene.amb_light.skybox_t->hdr, u
				* (minirt->scene.amb_light.skybox_t->hdr.width - 1), v
				* (minirt->scene.amb_light.skybox_t->hdr.height - 1)));
	}
	else
		return ((t_fcolor){0.0, 0.0, 0.0});
}

static t_fcolor	get_background_color_value(t_minirt *minirt, t_ray ray,
	char clamp)
{
	size_t	i;
	double	u;
	double	v;

	ray.dir = vec3_unit(ray.dir);
	v = 0.5 + asin(ray.dir.y) / PI_D;
	u = 0.5 - atan2(ray.dir.z, ray.dir.x) / (2.0 * PI_D);
	if (minirt->scene.amb_light.skybox_t->type == IMAGE
		&& minirt->scene.amb_light.skybox_t->img.rgba)
	{
		i = minirt->scene.amb_light.skybox_t->img.width
			* (int)(v * minirt->scene.amb_light.skybox_t->img.height)
			+ (int)(u * minirt->scene.amb_light.skybox_t->img.width);
		return (multiply_scalar_fcolor(
				rgba_to_fcolor(minirt->scene.amb_light.skybox_t->img.rgba[i]),
				minirt->scene.amb_light.ratio));
	}
	return (get_hdr_color(minirt, ray, clamp));
}

t_fcolor	get_background_color(t_minirt *minirt, t_ray ray)
{
	if (minirt->scene.amb_light.skybox_t == NULL)
		return (multiply_scalar_fcolor(
				color_to_fcolor(minirt->scene.amb_light.skybox_c),
				minirt->scene.amb_light.ratio));
	else
		return (get_background_color_value(minirt, ray, 0));
}

t_fcolor	get_background_color_clamp(t_minirt *minirt, t_ray ray)
{
	if (minirt->scene.amb_light.skybox_t == NULL)
		return (multiply_scalar_fcolor(
				color_to_fcolor(minirt->scene.amb_light.skybox_c),
				minirt->scene.amb_light.ratio));
	else
		return (get_background_color_value(minirt, ray, 1));
}

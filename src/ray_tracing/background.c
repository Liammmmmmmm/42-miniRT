/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:37:08 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/17 12:26:18 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_fcolor	get_background_color(t_minirt *minirt, t_ray ray)
{
	int		i;
	double	u;
	double	v;

	if (minirt->scene.amb_light.skybox_t == NULL)
		return (multiply_scalar_fcolor(color_to_fcolor(minirt->scene.amb_light.skybox_c), minirt->scene.amb_light.ratio));
	else
	{
		ray.dir = vec3_unit(ray.dir);
		v = 0.5 + asin(ray.dir.y) / PI_D;
		u = 0.5 - atan2(ray.dir.z, ray.dir.x) / (2.0 * PI_D);
		if (minirt->scene.amb_light.skybox_t->type == IMAGE && minirt->scene.amb_light.skybox_t->img.rgba)
		{
			i = minirt->scene.amb_light.skybox_t->img.width
				* (int)(v * minirt->scene.amb_light.skybox_t->img.height)
				+ (int)(u * minirt->scene.amb_light.skybox_t->img.width);
			return (multiply_scalar_fcolor(rgba_to_fcolor(minirt->scene.amb_light.skybox_t->img.rgba[i]), minirt->scene.amb_light.ratio));
		}
		else if (minirt->scene.amb_light.skybox_t->type == HDR && minirt->scene.amb_light.skybox_t->hdr.pixels)
			return (clamp_fcolor_val(get_hdr_pixel_skybox(minirt, &minirt->scene.amb_light.skybox_t->hdr, u * (minirt->scene.amb_light.skybox_t->hdr.width - 1), v * (minirt->scene.amb_light.skybox_t->hdr.height - 1)), 0.0, 16.0));
		else
		{
			return ((t_fcolor){0.0, 0.0, 0.0});
		}
	}
}

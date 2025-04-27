/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:37:08 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/27 18:14:05 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	get_default_color(t_ray ray)
{
	double	a;

	a = 0.5 * (ray.dir.y + 1);
	if (a > 1)
		a = 1;
	if (a < 0)
		a = 0;
	return ((t_color){((1 - a) * 255 + a * 128),
		((1 - a) * 255 + a * 178),
		((1 - a) * 255 + a * 255)});
}

t_color	get_background_color(t_minirt *minirt, t_ray ray)
{
	int		i;
	double	u;
	double	v;
	t_color	background;

	if (minirt->scene.amb_light.have_skybox == 0)
		background = get_default_color(ray);
	else if (minirt->scene.amb_light.skybox_t == NULL)
		return (minirt->scene.amb_light.skybox_c);
	else
	{
		ray.dir = vec3_unit(ray.dir);
		v = 0.5 + ray.dir.y * 0.5;
		u = 0.5 + atan2(ray.dir.x, ray.dir.z) / (2 * PI_D);
		i = minirt->scene.amb_light.skybox_t->img.width
			* (int)(v * minirt->scene.amb_light.skybox_t->img.height)
			+ (int)(u * minirt->scene.amb_light.skybox_t->img.width);
		return (minirt->scene.amb_light.skybox_t->img.pixel_data[i]);
	}
	return (background);
}

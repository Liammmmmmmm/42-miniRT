/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_sample.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:49:52 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/02 14:10:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	calc_one_sample_task(t_minirt *minirt, t_vec3 offset, t_uint i,
	int max_bounces)
{
	t_fcolor		color;
	t_ray			ray;

	if (minirt->scene.camera.defocus_angle <= 0)
		ray.orig = minirt->scene.camera.position;
	else
		ray.orig = defocus_disk_sample(minirt);
	ray.dir = vec3_subtract(vec3_add(vec3_add(minirt->viewport.pixel00_loc,
					vec3_multiply_scalar(minirt->viewport.pixel_delta_u,
						(i % minirt->viewport.render_w) + offset.x)),
				vec3_multiply_scalar(minirt->viewport.pixel_delta_v,
					(i / minirt->viewport.render_w) + offset.y)), ray.orig);
	if (minirt->scene.bvh.normal_mode)
		color = path_trace_normal(minirt, ray);
	else
		color = path_trace(minirt, ray, max_bounces);
	minirt->screen.float_render[i].r += color.r;
	minirt->screen.float_render[i].g += color.g;
	minirt->screen.float_render[i].b += color.b;
}

void	calc_one_sample(t_minirt *minirt, t_vec3 offset, int max_bounces)
{
	const t_uint	tpi = minirt->viewport.render_w * minirt->viewport.render_h;
	t_uint			i;

	i = 0;
	while (i < tpi)
	{
		calc_one_sample_task(minirt, offset, i, max_bounces);
		i++;
	}
}

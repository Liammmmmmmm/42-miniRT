/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_sample.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:49:52 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/19 13:18:50 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"



void	set_heat_map_color(t_minirt *minirt)
{
	const size_t	tpi = minirt->viewport.render_w * minirt->viewport.render_h;
	size_t			i;
	int				heat_map_max_value;

	i = 0;
	heat_map_max_value = 0;
	while (i < tpi)
	{
		if (heat_map_max_value < minirt->viewport.depth_buffer[i])
			heat_map_max_value = minirt->viewport.depth_buffer[i];
		i++;
	}
	i = 0;
	while (i < tpi)
	{
		if (minirt->viewport.depth_buffer[i] >= 0)
		{
			minirt->screen.float_render[i] = \
				add_fcolor(minirt->screen.float_render[i], \
					get_heatmap_fcolor(minirt->viewport.depth_buffer[i], \
						heat_map_max_value));
		}
		i++;
	}
}

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
	if (minirt->render_mode == 2)
		color = path_trace_normal(minirt, ray);
	else if (minirt->render_mode == 1)
		minirt->viewport.depth_buffer[i] = path_trace_bvh(minirt, ray);
	else
		color = path_trace(minirt, ray, max_bounces);
	if (minirt->render_mode != 1)
	{
		minirt->screen.float_render[i].r += color.r;
		minirt->screen.float_render[i].g += color.g;
		minirt->screen.float_render[i].b += color.b;
	}
}

void	calc_one_sample(t_minirt *minirt, t_vec3 offset, int max_bounces)
{
	const t_uint	tpi = minirt->viewport.render_w * minirt->viewport.render_h;
	t_uint			i;

	i = 0;
	if (minirt->screen.sample == 0 || (get_cpu_time() \
	- minirt->screen.first_sample_time) / minirt->screen.sample
		> SAMPLE_PROGRESS_BAR_TIME)
	{
		print_progress_bar(0, tpi);
		while (i < tpi)
		{
			calc_one_sample_task(minirt, offset, i++, max_bounces);
			print_progress_bar(i, tpi);
		}
		if (get_cpu_time() - minirt->screen.last_sample_time
			< SAMPLE_PROGRESS_BAR_TIME)
			write_no_err(1, "\r\e[K", 4);
		else
			write_no_err(1, " ", 1);
	}
	else
		while (i < tpi)
			calc_one_sample_task(minirt, offset, i++, max_bounces);
	if (minirt->render_mode == 1)
		set_heat_map_color(minirt);
}

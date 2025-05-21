/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 15:07:30 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "material.h"
#include <math.h>

void	calc_one_sample(t_minirt *minirt, t_vec3 offset)
{
	t_fcolor			color;
	const t_uint	tpi = minirt->viewport.render_w * minirt->viewport.render_h;
	t_uint			i;
	t_ray			ray;

	i = 0;
	while (i < tpi)
	{
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
			color = path_trace(minirt, ray, 5);
		minirt->screen.float_render[i].r += color.r;
		minirt->screen.float_render[i].g += color.g;
		minirt->screen.float_render[i].b += color.b;
		i++;
	}
	if (minirt->controls.selected_x != -1 && minirt->controls.selected_y != -1)
	{
		ft_bzero(minirt->controls.traced_ray, sizeof(t_vec3) * 11);
		if (minirt->scene.camera.defocus_angle <= 0)
			ray.orig = minirt->scene.camera.position;
		else
			ray.orig = defocus_disk_sample(minirt);
		ray.dir = vec3_subtract(vec3_add(vec3_add(minirt->viewport.pixel00_loc,
						vec3_multiply_scalar(minirt->viewport.pixel_delta_u, minirt->controls.selected_x + offset.x)),
					vec3_multiply_scalar(minirt->viewport.pixel_delta_v, minirt->controls.selected_y + offset.y)), ray.orig);
		
		debug_path_trace(minirt, ray, 8);

	}
}

void	draw_pixels(t_minirt *minirt)
{
	t_vec3	offset;

	minirt->screen.last_sample_time = get_cpu_time();
	offset = vec3_random();
	calc_one_sample(minirt, offset);
	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	put_render_to_buff(minirt);
	if (minirt->options.no_display)
		return ;
	if (minirt->controls.selected_x != -1 && minirt->controls.selected_y != -1)
		debug_ray(minirt);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
	printf("Sample %d - %zums\n", minirt->screen.sample, get_cpu_time() - minirt->screen.last_sample_time);
}

void	check_sample_amount(t_minirt *minirt)
{
	if (minirt->screen.sample == minirt->screen.spp)
	{
		if (minirt->options.auto_export)
		{
			char *filename;

			if (minirt->options.anim.enabled && minirt->options.anim.frame_i < minirt->options.anim.frames)
				filename = ft_sprintf("%sminirt_export_SCENE_NAME.FRAME.%u.SAMPLES.%d.%u.ppm", minirt->options.output_dir, minirt->options.anim.frame_i, minirt->screen.sample, (unsigned int)get_cpu_time());
			else
				filename = ft_sprintf("%sminirt_export_SCENE_NAME.SAMPLES.%d.%u.ppm", minirt->options.output_dir, minirt->screen.sample, (unsigned int)get_cpu_time());
			printf("Start image export\n");
			if (filename)
				export_ppm_p6_minirt(filename, minirt);
			free(filename);
		}
		minirt->screen.sample = 0;
		minirt->screen.start_render = 0;
		if (minirt->options.anim.enabled && minirt->options.anim.frame_i < minirt->options.anim.frames)
		{
			minirt->options.anim.frame_i++;
			minirt->screen.start_render = 1;
		}
	}
}

void	render(t_minirt *minirt)
{
	if (!minirt->screen.start_render || minirt->screen.pause_render)
		return ;
	if (minirt->screen.sample == 0)
	{
		if (minirt->screen.sample_total_anim == 0 || minirt->options.anim.enabled == 0)
			minirt->screen.first_sample_time = get_cpu_time();
		init_animated_items(minirt);
		minirt->viewport = init_viewport(minirt);
		if (!minirt->options.no_display)
			ft_izero(minirt->screen.render, minirt->scene.win_width * minirt->scene.win_height);
		ft_bzero(minirt->screen.float_render, sizeof(t_fcolor) * minirt->viewport.render_w * minirt->viewport.render_h);
	}
	draw_pixels(minirt);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/29 10:17:32 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "material.h"
#include <math.h>

void	put_render_to_buff_upscaling(t_minirt *minirt)
{
	if ((long)minirt->controls.selected_upscaling == 0)
		put_render_to_buff(minirt);
	else if ((long)minirt->controls.selected_upscaling == 1)
		bilinear_upscale(minirt);
	else if ((long)minirt->controls.selected_upscaling == 2)
		bicubic_upscale(minirt);
}

static void	draw_pixels(t_minirt *minirt)
{
	t_vec3	offset;

	minirt->screen.last_sample_time = get_cpu_time();
	offset = vec3_random();
	calc_one_sample(minirt, offset, (int)minirt->controls.max_bounces);
	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	put_render_to_buff_upscaling(minirt);
	if (minirt->options.no_display)
		return ;
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
	printf("Sample %d - %zums\n", minirt->screen.sample, get_cpu_time()
		- minirt->screen.last_sample_time);
}

void	auto_export(t_minirt *minirt)
{
	char	*filename;

	if (minirt->options.anim.enabled && minirt->options.anim.frame_i
		< minirt->options.anim.frames)
		filename \
	= ft_sprintf("%sminirt_export_%s.FRAME.%u.SAMPLES.%d.%u.ppm",
			minirt->options.output_dir, minirt->scene.name,
			minirt->options.anim.frame_i, minirt->screen.sample,
			(unsigned int)get_cpu_time());
	else
		filename \
	= ft_sprintf("%sminirt_export_%s.SAMPLES.%d.%u.ppm",
			minirt->options.output_dir, minirt->scene.name,
			minirt->screen.sample, (unsigned int)get_cpu_time());
	printf("Start image export\n");
	if (filename)
		export_ppm_p6_minirt(filename, minirt);
	free(filename);
}

void	check_sample_amount(t_minirt *minirt)
{
	if (minirt->screen.sample == minirt->screen.spp)
	{
		if (minirt->options.auto_export)
			auto_export(minirt);
		minirt->screen.sample = 0;
		minirt->screen.start_render = 0;
		if (minirt->options.anim.enabled && minirt->options.anim.frame_i
			< minirt->options.anim.frames)
		{
			minirt->options.anim.frame_i++;
			minirt->screen.start_render = 1;
		}
	}
}

void	render_cpu(t_minirt *minirt)
{
	manage_movements(minirt);
	if (!minirt->screen.start_render || minirt->screen.pause_render)
		return ;
	if (minirt->screen.sample == 0)
	{
		if (minirt->screen.sample_total_anim == 0
			|| minirt->options.anim.enabled == 0)
			minirt->screen.first_sample_time = get_cpu_time();
		init_animated_items(minirt);
		minirt->viewport = init_viewport(minirt);
		if (!minirt->options.no_display)
			ft_izero(minirt->screen.render, minirt->scene.win_width
				* minirt->scene.win_height);
		ft_bzero(minirt->screen.float_render, sizeof(t_fcolor)
			* minirt->viewport.render_w * minirt->viewport.render_h);
	}
	draw_pixels(minirt);
}

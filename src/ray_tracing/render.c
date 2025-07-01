/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/01 17:03:59 by madelvin         ###   ########.fr       */
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

void	draw_pixels(t_minirt *minirt)
{
	t_vec3	offset;
	size_t		i;
	t_point	p1;
	t_point	p2;

	minirt->screen.last_sample_time = get_cpu_time();
	offset = vec3_random();
	calc_one_sample(minirt, offset, (int)minirt->controls.max_bounces);
	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	put_render_to_buff_upscaling(minirt);
	if (minirt->options.no_display)
		return ;
	// i = minirt->i;
	// i = 0;
	// printf("%d ---- %d-------------%f/%f/%f----%f/%f/%f----\n", i, minirt->scene.photon_map.photons[i].i, minirt->scene.photon_map.photons[i].p0.x, minirt->scene.photon_map.photons[i].p0.y, minirt->scene.photon_map.photons[i].p0.z, minirt->scene.photon_map.photons[i].p1.x, minirt->scene.photon_map.photons[i].p1.y, minirt->scene.photon_map.photons[i].p1.z);
	// while (i < minirt->scene.photon_map.photon_count)
	// {
	// 	project_vertex(minirt, minirt->scene.photon_map.photons[i].initial, &p1.x, &p1.y);
	// 	project_vertex(minirt, minirt->scene.photon_map.photons[i].p0, &p2.x, &p2.y);
	// 	draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	if (minirt->scene.photon_map.photons[i].i > 0)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p0, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p1, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 1)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p1, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p2, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, p1.color);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 2)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p2, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p3, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 3)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p3, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p4, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 4)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p4, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p5, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 5)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p5, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p6, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 6)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p6, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p7, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 7)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p7, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p8, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 8)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p8, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p9, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	if (minirt->scene.photon_map.photons[i].i > 9)
	// 	{
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p9, &p1.x, &p1.y);
	// 		project_vertex(minirt, minirt->scene.photon_map.photons[i].p10, &p2.x, &p2.y);
	// 		draw_line(&p1, &p2, &minirt->mlx.img, 0x000000);
	// 	}
	// 	i++;
	// }
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

void	render(t_minirt *minirt)
{
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_gpu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/29 10:18:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"
#include <string.h>

static void	copy_buffer_to_backup(t_minirt *minirt, t_fcolor *src)
{
	int	total;
	int	i;
	int	divide;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	i = 0;
	while (i < total)
	{
		minirt->screen.float_render_backup[i].r = src[i].r;
		minirt->screen.float_render_backup[i].g = src[i].g;
		minirt->screen.float_render_backup[i].b = src[i].b;
		i++;
	}
}

static void	prepare_display_buffer(t_minirt *minirt)
{
	int	total;

	total = minirt->scene.render_width * minirt->scene.render_height;
	if (minirt->render_mode == 3)
		copy_buffer_to_backup(minirt, minirt->screen.albedo_buffer);
	else if (minirt->render_mode == 4)
		copy_buffer_to_backup(minirt, minirt->screen.normal_buffer);
	else if (minirt->render_mode == 5)
	{
		int	i;
		int	divide;

		divide = minirt->screen.last_sample_am;
		if (divide == 0)
			divide = 1;
		i = 0;
		while (i < total)
		{
			float depth = minirt->screen.depth_buffer_denoise[i] / divide;
			float normalized = 1.0f / (1.0f + depth * 0.1f);
			minirt->screen.float_render_backup[i].r = normalized * divide;
			minirt->screen.float_render_backup[i].g = normalized * divide;
			minirt->screen.float_render_backup[i].b = normalized * divide;
			i++;
		}
	}
	else
		memcpy(minirt->screen.float_render_backup, minirt->screen.float_render,
			total * sizeof(t_fcolor));
	if (minirt->show_denoised && minirt->denoiser && minirt->denoiser->available
		&& minirt->render_mode == 0)
		apply_denoising_forced(minirt);
}

static void	draw_pixels(t_minirt *minirt)
{
	minirt->screen.last_sample_time = get_cpu_time();
	compute_frame_gpu(minirt);
	if (minirt->render_mode == 1)
		set_heat_map_color(minirt);
	prepare_display_buffer(minirt);
	if (minirt->options.no_display || minirt->options.client.enabled)
		return ;
	put_render_to_buff_upscaling(minirt);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
	printf("Sample %d - %zums\n", minirt->screen.sample, get_cpu_time()
		- minirt->screen.last_sample_time);
}

void	render_gpu(t_minirt *minirt)
{
	t_bool		build_scene_gpu;

	manage_movements(minirt);
	if (!minirt->screen.start_render || minirt->screen.pause_render)
		return ;
	if (minirt->screen.sample != 0)
		return (draw_pixels(minirt));
	if (minirt->screen.sample_total_anim == 0
		|| minirt->options.anim.enabled == 0)
		minirt->screen.first_sample_time = get_cpu_time();
	init_animated_items(minirt);
	build_scene_gpu = minirt->scene.build_bvh;
	minirt->viewport = init_viewport(minirt);
	if (!minirt->options.no_display && !minirt->options.client.enabled)
		ft_izero(minirt->screen.render,
			minirt->scene.win_width * minirt->scene.win_height);
	init_float_render(minirt);
	if (build_scene_gpu)
		convert_scene_build(minirt, &minirt->scene, &minirt->viewport,
			&minirt->shaders_data.scene);
	else
		convert_scene(minirt, &minirt->scene, &minirt->viewport,
			&minirt->shaders_data.scene);
	draw_pixels(minirt);
}

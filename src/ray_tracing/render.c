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

static void	copy_to_display_buffer(t_minirt *minirt)
{
	int	total;

	total = minirt->scene.render_width * minirt->scene.render_height;
	if (minirt->render_mode == 3)
	{
		printf("[DEBUG] Mode 3 (Albedo) - calling copy_buffer_to_backup\n");
		copy_buffer_to_backup(minirt, minirt->screen.albedo_buffer);
	}
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
			float depth = minirt->screen.depth_buffer_denoise[i];
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
}

static void	prepare_display_buffer(t_minirt *minirt)
{
	copy_to_display_buffer(minirt);
	if (minirt->show_denoised && minirt->denoiser && minirt->denoiser->available
		&& minirt->render_mode == 0)
		apply_denoising_forced(minirt);
}

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
	prepare_display_buffer(minirt);
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
	char	*filename_noisy;
	char	*filename_denoised;
	bool	was_enabled;
	unsigned int	timestamp;

	timestamp = (unsigned int)get_cpu_time();

	was_enabled = minirt->show_denoised;
	minirt->show_denoised = false;

	if (minirt->options.anim.enabled && minirt->options.anim.frame_i
		< minirt->options.anim.frames)
		filename_noisy = ft_sprintf("%sminirt_export_%s.FRAME.%u.SAMPLES.%d.%u.NOISY.ppm",
			minirt->options.output_dir, minirt->scene.name,
			minirt->options.anim.frame_i, minirt->screen.sample, timestamp);
	else
		filename_noisy = ft_sprintf("%sminirt_export_%s.SAMPLES.%d.%u.NOISY.ppm",
			minirt->options.output_dir, minirt->scene.name,
			minirt->screen.sample, timestamp);

	printf("Exporting noisy image...\n");
	if (filename_noisy)
		export_ppm_p6_minirt(filename_noisy, minirt);
	free(filename_noisy);

	if (minirt->denoiser && minirt->denoiser->available)
	{
		minirt->show_denoised = true;

		if (minirt->options.anim.enabled && minirt->options.anim.frame_i
			< minirt->options.anim.frames)
			filename_denoised = ft_sprintf("%sminirt_export_%s.FRAME.%u.SAMPLES.%d.%u.DENOISED.ppm",
				minirt->options.output_dir, minirt->scene.name,
				minirt->options.anim.frame_i, minirt->screen.sample, timestamp);
		else
			filename_denoised = ft_sprintf("%sminirt_export_%s.SAMPLES.%d.%u.DENOISED.ppm",
				minirt->options.output_dir, minirt->scene.name,
				minirt->screen.sample, timestamp);

		printf("Exporting denoised image...\n");
		if (filename_denoised)
			export_ppm_p6_minirt(filename_denoised, minirt);
		free(filename_denoised);
	}

	// Restaurer l'état original
	minirt->show_denoised = was_enabled;
	printf("Export complete\n");
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
		ft_bzero(minirt->screen.albedo_buffer, sizeof(t_fcolor)
			* minirt->viewport.render_w * minirt->viewport.render_h);
		ft_bzero(minirt->screen.normal_buffer, sizeof(t_fcolor)
			* minirt->viewport.render_w * minirt->viewport.render_h);
		ft_bzero(minirt->screen.depth_buffer_denoise, sizeof(float)
			* minirt->viewport.render_w * minirt->viewport.render_h);
	}
	draw_pixels(minirt);
}

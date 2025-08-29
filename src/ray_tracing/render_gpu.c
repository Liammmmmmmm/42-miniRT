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
#include "bvh.h"
#include "material.h"
#include "camera.h"
#include <math.h>

static void	draw_pixels(t_minirt *minirt)
{
	minirt->screen.last_sample_time = get_cpu_time();
	compute_frame_gpu(minirt);
	if (minirt->render_mode == 1)
		set_heat_map_color(minirt);
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

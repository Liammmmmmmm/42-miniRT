/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/08 11:10:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"
#include "camera.h"
#include "network.h"

void	render_frame_server(t_minirt *minirt)
{
	static int	last_sample_amount = -1;


	pthread_mutex_lock(&minirt->screen.sample_mutex);
	if (minirt->screen.sample >= minirt->screen.spp
		&& minirt->screen.client_sample_exceed
		&& minirt->screen.client_sample_exceed + CLIENT_ACCUMULATION_TIME - 5000 < get_cpu_time())
	{
		minirt->screen.client_sample_exceed = 0;
		if (minirt->options.auto_export)
			auto_export(minirt);
		minirt->screen.sample = 0;
		if (minirt->options.anim.enabled && minirt->options.anim.frame_i
			< minirt->options.anim.frames)
		{
			ft_bzero(minirt->screen.float_render, sizeof(t_fcolor) * minirt->scene.render_height * minirt->scene.render_width);
			minirt->options.anim.frame_i++;
		}
		else
			mlx_loop_end(minirt->mlx.mlx);
	}
	pthread_mutex_unlock(&minirt->screen.sample_mutex);

	if (exit_if_anim_finished(minirt))
		return ;
	pthread_mutex_lock(&minirt->screen.sample_mutex);
	if (last_sample_amount != minirt->screen.sample)
	{
		put_render_to_buff(minirt);
		last_sample_amount = minirt->screen.sample;
		pthread_mutex_unlock(&minirt->screen.sample_mutex);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
			minirt->mlx.img.img, 0, 0);
	}
	else
		pthread_mutex_unlock(&minirt->screen.sample_mutex);

	if (minirt->options.no_display)
		no_display_enable(minirt);
	
	// if (minirt->options.no_display) // Idealement faire un nodisplay special serveur
	// 	no_display_enable(minirt);
	
	minirt->stats.frame += 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/05 13:46:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"
#include "camera.h"

void	render_frame_server(t_minirt *minirt)
{
	static int	last_sample_amount = -1;

	check_sample_amount(minirt);
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
	
	// if (minirt->options.no_display) // Idealement faire un nodisplay special serveur
	// 	no_display_enable(minirt);
	
	minirt->stats.frame += 1;
}

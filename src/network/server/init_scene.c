/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:47:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 11:50:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "minirt.h"

void	init_scene_server(t_minirt *minirt)
{
	if (minirt->screen.sample_total_anim == 0
		|| minirt->options.anim.enabled == 0)
		minirt->screen.first_sample_time = get_cpu_time();
	init_animated_items(minirt);
	minirt->viewport = init_viewport(minirt);
	// if (!minirt->options.no_display)
	// 	ft_izero(minirt->screen.render, minirt->scene.win_width
	// 		* minirt->scene.win_height);
	ft_bzero(minirt->screen.float_render, sizeof(t_fcolor)
		* minirt->viewport.render_w * minirt->viewport.render_h);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:47:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 09:03:02 by lilefebv         ###   ########lyon.fr   */
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
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_new_win_size_gpu.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:55:39 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 18:10:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"

void	try_set_new_size(t_minirt *minirt)
{
	t_fcolor	*new_render;

	new_render = malloc(sizeof(t_fcolor) * minirt->scene.render_width
			* minirt->scene.render_height);
	if (new_render)
	{
		delete_ssbo(&minirt->shaders_data.ssbo);
		init_ssbo(&minirt->shaders_data, minirt->scene.render_width,
			minirt->scene.render_height);
		free(minirt->screen.float_render);
		minirt->screen.float_render = new_render;
	}
}

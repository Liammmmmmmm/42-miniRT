/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_new_win_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:55:39 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/23 11:57:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	try_set_new_size(t_minirt *minirt)
{
	t_fcolor	*new_render;

	new_render = malloc(sizeof(t_fcolor) * minirt->scene.render_width
			* minirt->scene.render_height);
	if (new_render)
	{
		free(minirt->screen.float_render);
		minirt->screen.float_render = new_render;
	}
}

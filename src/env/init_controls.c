/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_controls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:33:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/23 13:19:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_controls(t_minirt *minirt)
{
	ft_bzero(&minirt->controls.keydown, sizeof(t_keydown));
	minirt->controls.open_controls = KEY_C;
	minirt->controls.selected_x = -1;
	minirt->controls.selected_y = -1;
	minirt->controls.max_bounces = 10;
	minirt->controls.res_render_x = minirt->scene.render_width;
	minirt->controls.res_render_y = minirt->scene.render_height;
	minirt->viewport.gamma = 1.0;
	minirt->controls.movements.speed_modifier = 10;
}

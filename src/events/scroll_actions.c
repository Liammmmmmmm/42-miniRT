/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:24:18 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 18:24:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	scroll_speed_modifier(int key, t_minirt *minirt)
{
	if (key == SCROLL_DOWN)
	{
		minirt->controls.movements.speed_modifier -= 1;
		if (minirt->controls.movements.speed_modifier < 1)
			minirt->controls.movements.speed_modifier = 1;
	}
	else if (key == SCROLL_UP)
	{
		minirt->controls.movements.speed_modifier += 1;
		if (minirt->controls.movements.speed_modifier > 100)
			minirt->controls.movements.speed_modifier = 100;
	}
}

void	scroll_fov_modifier(int key, t_minirt *minirt)
{
	if (key == SCROLL_DOWN)
	{
		minirt->controls.values.fov -= 1;
		if (minirt->controls.values.fov < 1)
			minirt->controls.values.fov = 1;
		restart_minirt(minirt);
	}
	else if (key == SCROLL_UP)
	{
		minirt->controls.values.fov += 1;
		if (minirt->controls.values.fov > 180)
			minirt->controls.values.fov = 180;
		restart_minirt(minirt);
	}
}

void	mouse_scroll(int key, t_minirt *minirt)
{
	if (minirt->controls.keydown.lctrl)
		scroll_speed_modifier(key, minirt);
	else
		scroll_fov_modifier(key, minirt);
}

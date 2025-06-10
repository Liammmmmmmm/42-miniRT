/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:42:54 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 16:56:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "minirt.h"

int	color_picker_stop_minirt(t_minirt *minirt, int x, int y, t_color_picker *cp)
{
	const int	last_val = cp->btn.background_color;

	if (color_picker_action(cp, x, y))
	{
		if (last_val != cp->btn.background_color)
			stop_minirt(minirt);
		return (1);
	}
	return (0);
}

int	color_picker_stop_minirt_mat(t_minirt *minirt, int x, int y,
	t_color_picker *cp)
{
	const int	last_val = cp->btn.background_color;

	if (color_picker_action(cp, x, y))
	{
		if (last_val != cp->btn.background_color)
		{
			minirt->micrort.sample = 0;
			stop_minirt(minirt);
		}
		return (1);
	}
	return (0);
}

void	float_input_position(t_minirt *minirt, int x, int y)
{
	float_input_focus(&minirt->controls.float_input[0], x, y);
	float_input_focus(&minirt->controls.float_input[1], x, y);
	float_input_focus(&minirt->controls.float_input[2], x, y);
}

void	float_input_orientation(t_minirt *minirt, int x, int y)
{
	float_input_focus(&minirt->controls.float_input[3], x, y);
	float_input_focus(&minirt->controls.float_input[4], x, y);
	float_input_focus(&minirt->controls.float_input[5], x, y);
}

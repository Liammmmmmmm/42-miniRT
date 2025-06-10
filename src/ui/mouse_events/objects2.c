/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:41:04 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 14:51:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

int	mouse_down_light(int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	return (0);
}

int	mouse_down_cone(int key, int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[0]))
		return (1);
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[1]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_orientation(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	float_input_focus(&minirt->controls.float_input[7], x, y);
	return (0);
}

int	mouse_down_hyperboloid(int key, int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[0]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_orientation(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	float_input_focus(&minirt->controls.float_input[7], x, y);
	float_input_focus(&minirt->controls.float_input[8], x, y);
	float_input_focus(&minirt->controls.float_input[9], x, y);
	float_input_focus(&minirt->controls.float_input[10], x, y);
	return (0);
}

int	mouse_down_custom(int key, int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[0]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_orientation(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	float_input_focus(&minirt->controls.float_input[7], x, y);
	float_input_focus(&minirt->controls.float_input[8], x, y);
	return (0);
}

int	mouse_down_directional_light(int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	float_input_orientation(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	return (0);
}

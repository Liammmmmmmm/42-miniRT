/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:39:51 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 14:42:01 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "minirt.h"

int	mouse_down_sphere(int key, int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[0]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	return (0);
}

int	mouse_down_cylinder(int key, int x, int y, t_minirt *minirt)
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
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[2]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_orientation(minirt, x, y);
	float_input_focus(&minirt->controls.float_input[6], x, y);
	float_input_focus(&minirt->controls.float_input[7], x, y);
	return (0);
}

int	mouse_down_plane(int key, int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt(minirt, x, y,
			&minirt->controls.color_picker[0]))
		return (1);
	if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[0]))
		return (1);
	float_input_position(minirt, x, y);
	float_input_orientation(minirt, x, y);
	return (0);
}

int	mouse_down_selected_object2(int key, int x, int y, t_minirt *minirt)
{
	if (minirt->controls.ui_infos.selected_object->type == HYPERBOLOID
		&& mouse_down_hyperboloid(key, x, y, minirt))
		return (1);
	else if (minirt->controls.ui_infos.selected_object->type == CUSTOM
		&& mouse_down_custom(key, x, y, minirt))
		return (1);
	else if (minirt->controls.ui_infos.selected_object->type
		== DIRECTIONAL_LIGHT && mouse_down_directional_light(x, y, minirt))
		return (1);
	return (0);
}

int	mouse_down_selected_object(int key, int x, int y, t_minirt *minirt)
{
	if (manage_scroll_selected_object(key, x, y, minirt))
		return (1);
	if (key != LEFT_CLICK)
		return (0);
	if (minirt->controls.ui_infos.selected_object->type == SPHERE
		&& mouse_down_sphere(key, x, y, minirt))
		return (1);
	else if (minirt->controls.ui_infos.selected_object->type == CYLINDER
		&& mouse_down_cylinder(key, x, y, minirt))
		return (1);
	else if (minirt->controls.ui_infos.selected_object->type == PLANE
		&& mouse_down_plane(key, x, y, minirt))
		return (1);
	else if (minirt->controls.ui_infos.selected_object->type == LIGHT
		&& mouse_down_light(x, y, minirt))
		return (1);
	else if (minirt->controls.ui_infos.selected_object->type == CONE
		&& mouse_down_cone(key, x, y, minirt))
		return (1);
	else if (mouse_down_selected_object2(key, x, y, minirt))
		return (1);
	return (0);
}

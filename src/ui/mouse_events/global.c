/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:31:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 14:47:22 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "minirt.h"

static void	left_part_floats_input(int x, int y, t_minirt *minirt)
{
	float_input_focus(&minirt->controls.float_input[19], x, y);
	float_input_focus(&minirt->controls.float_input[20], x, y);
	float_input_focus(&minirt->controls.float_input[21], x, y);
	float_input_focus(&minirt->controls.float_input[22], x, y);
	float_input_focus(&minirt->controls.float_input[23], x, y);
	float_input_focus(&minirt->controls.float_input[24], x, y);
	float_input_focus(&minirt->controls.float_input[25], x, y);
	float_input_focus(&minirt->controls.float_input[26], x, y);
	float_input_focus(&minirt->controls.float_input[27], x, y);
	float_input_focus(&minirt->controls.float_input[28], x, y);
	float_input_focus(&minirt->controls.float_input[29], x, y);
	float_input_focus(&minirt->controls.float_input[30], x, y);
	float_input_focus(&minirt->controls.float_input[31], x, y);
}

/**
 * 0, 1, 2 : Start, stop, pause
 * 3, 4 : Tabs objects/materials
 */
int	left_and_global_left_click(int key, int x, int y, t_minirt *minirt)
{
	t_controls	*controls;

	controls = &minirt->controls;
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&controls->dropdown[12]))
		return (1);
	if (mouse_down_dropdown(key, (t_point2){x, y}, &controls->dropdown[13]))
		return (1);
	if (key != LEFT_CLICK)
		return (0);
	button_action(&controls->buttons[0], x, y, &controls->btn_clicked);
	button_action(&controls->buttons[1], x, y, &controls->btn_clicked);
	button_action(&controls->buttons[2], x, y, &controls->btn_clicked);
	button_action(&controls->buttons[3], x, y, &controls->btn_clicked);
	button_action(&controls->buttons[4], x, y, &controls->btn_clicked);
	slider_mouse_down(&controls->sliders[0], x, y);
	left_part_floats_input(x, y, minirt);
	if (color_picker_stop_minirt(minirt, x, y, &controls->color_picker[3]))
		return (1);
	return (0);
}

int	manage_scroll_selected_object(int key, int x, int y, t_minirt *minirt)
{
	if ((key == SCROLL_DOWN || key == SCROLL_UP)
		&& minirt->controls.ui_infos.selected_object->type != DIRECTIONAL_LIGHT
		&& minirt->controls.ui_infos.selected_object->type != LIGHT)
	{
		if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
			&minirt->controls.dropdown[0]))
			return (1);
		if (minirt->controls.ui_infos.selected_object->type == CYLINDER)
		{
			if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
				&minirt->controls.dropdown[1]))
				return (1);
			if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
				&minirt->controls.dropdown[2]))
				return (1);
		}
		else if (minirt->controls.ui_infos.selected_object->type == CONE)
		{
			if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y},
				&minirt->controls.dropdown[1]))
				return (1);
		}
	}
	return (0);
}

int	mouse_down_object_tab(int key, int x, int y, t_minirt *minirt)
{
	if (mouse_down_obj(minirt, key, x, y))
		return (1);
	if (mouse_down_dropdown(key, (t_point2){x, y},
		&minirt->controls.dropdown[11]))
		return (1);
	if (key == LEFT_CLICK)
	{
		button_action(&minirt->controls.buttons[5], x, y,
			&minirt->controls.btn_clicked);
		button_action(&minirt->controls.buttons[6], x, y,
			&minirt->controls.btn_clicked);
	}
	if (minirt->controls.ui_infos.selected_object)
		if (mouse_down_selected_object(key, x, y, minirt))
			return (1);
	return (0);
}

void	layout_mouse_down(int key, int x, int y, t_minirt *minirt)
{
	left_and_global_left_click(key, x, y, minirt);
	if (minirt->controls.ui_infos.tab_selected == 0)
	{
		if (mouse_down_object_tab(key, x, y, minirt))
			return ;
	}
	else if (minirt->controls.ui_infos.tab_selected == 1)
	{
		if (material_tab_events(key, x, y, minirt))
			return ;
	}
}

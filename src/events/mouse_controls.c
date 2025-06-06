/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:41:18 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/06 15:45:55 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	mouse_add_mat(int key, int x, int y)
{
	if (key == LEFT_CLICK && x > 300 && x <= 600 && y > 270 && y < 300)
		return (1);
	return (0);
}

void	layout_mouse_down(int key, int x, int y, t_minirt *minirt)
{
	if (key == LEFT_CLICK)
	{
		// START / STOP / PAUSE
		button_action(&minirt->controls.buttons[0], x, y, &minirt->controls.btn_clicked);
		button_action(&minirt->controls.buttons[1], x, y, &minirt->controls.btn_clicked);
		button_action(&minirt->controls.buttons[2], x, y, &minirt->controls.btn_clicked);

		// TABS: Objects | Materials
		button_action(&minirt->controls.buttons[3], x, y, &minirt->controls.btn_clicked);
		button_action(&minirt->controls.buttons[4], x, y, &minirt->controls.btn_clicked);

		// Basics 4 sliders
		slider_mouse_down(&minirt->controls.sliders[0], x, y);
		// slider_mouse_down(&minirt->controls.sliders[1], x, y);
		// slider_mouse_down(&minirt->controls.sliders[2], x, y);
		// slider_mouse_down(&minirt->controls.sliders[3], x, y);
		// slider_mouse_down(&minirt->controls.sliders[4], x, y);
		// slider_mouse_down(&minirt->controls.sliders[5], x, y);
		float_input_focus(&minirt->controls.float_input[19], x, y);
		float_input_focus(&minirt->controls.float_input[20], x, y);
		float_input_focus(&minirt->controls.float_input[21], x, y);
		float_input_focus(&minirt->controls.float_input[22], x, y);
		float_input_focus(&minirt->controls.float_input[23], x, y);
		float_input_focus(&minirt->controls.float_input[24], x, y);
		float_input_focus(&minirt->controls.float_input[25], x, y);
		float_input_focus(&minirt->controls.float_input[26], x, y);
		int last_val = minirt->controls.color_picker[3].btn.background_color;
		if (color_picker_action(&minirt->controls.color_picker[3], x, y))
		{
			if (last_val != minirt->controls.color_picker[3].btn.background_color)
				stop_minirt(minirt);
			return ;
		}
		if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[12]))
			return ;
		if (mouse_down_dropdown(key, (t_point2){x, y}, &minirt->controls.dropdown[13]))
			return ;
		float_input_focus(&minirt->controls.float_input[27], x, y);
		float_input_focus(&minirt->controls.float_input[28], x, y);
		float_input_focus(&minirt->controls.float_input[29], x, y);
		float_input_focus(&minirt->controls.float_input[30], x, y);
		float_input_focus(&minirt->controls.float_input[31], x, y);
	}
	
	if (minirt->controls.ui_infos.tab_selected == 0)
	{
		if (mouse_down_obj(minirt, key, x, y))
			return ;
		if (mouse_down_dropdown(key, (t_point2){x, y}, &minirt->controls.dropdown[11]))
			return ;
		if (key == LEFT_CLICK)
		{
			button_action(&minirt->controls.buttons[5], x, y, &minirt->controls.btn_clicked);
			button_action(&minirt->controls.buttons[6], x, y, &minirt->controls.btn_clicked);
		}
		if (minirt->controls.ui_infos.selected_object)
		{
			if (key == LEFT_CLICK)
			{
				if (minirt->controls.ui_infos.selected_object->type == SPHERE)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == CYLINDER)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[1]))
						return ;
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[2]))
						return ;
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[3], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[4], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[5], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[7], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == PLANE)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[3], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[4], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[5], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == LIGHT)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == CONE)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[1]))
						return ;
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[3], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[4], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[5], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[7], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == HYPERBOLOID)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[3], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[4], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[5], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[7], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[8], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[9], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[10], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == CUSTOM)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (float_input_focus(&minirt->controls.float_input[0], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[1], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[2], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[3], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[4], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[5], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[7], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[8], x, y))
						return ;
				}
				else if (minirt->controls.ui_infos.selected_object->type == DIRECTIONAL_LIGHT)
				{
					int last_val = minirt->controls.color_picker[0].btn.background_color;
					if (color_picker_action(&minirt->controls.color_picker[0], x, y))
					{
						if (last_val != minirt->controls.color_picker[0].btn.background_color)
							stop_minirt(minirt);
						return ;
					}
					if (float_input_focus(&minirt->controls.float_input[3], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[4], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[5], x, y))
						return ;
					if (float_input_focus(&minirt->controls.float_input[6], x, y))
						return ;
				}
			}
			else if (key == SCROLL_DOWN || key == SCROLL_UP)
			{
				if (minirt->controls.ui_infos.selected_object->type != LIGHT)
				{
					if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
						return ;
					if (minirt->controls.ui_infos.selected_object->type == CYLINDER)
					{
						if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[1]))
							return ;
						if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[2]))
							return ;
					}
					else if (minirt->controls.ui_infos.selected_object->type == CONE)
					{
						if (mouse_down_dropdown_mat(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[1]))
							return ;
					}
				}
			}
		}
	}
	else if (minirt->controls.ui_infos.tab_selected == 1)
	{
		if (mouse_down_mat(minirt, key, x, y))
			return ;
		if (mouse_add_mat(key, x, y))
			add_material(minirt);
		if (minirt->controls.ui_infos.selected_material)
		{
			int last_val = minirt->controls.color_picker[1].btn.background_color;
			if (color_picker_action(&minirt->controls.color_picker[1], x, y))
			{
				if (last_val != minirt->controls.color_picker[1].btn.background_color)
				{
					minirt->micrort.sample = 0;
					stop_minirt(minirt);
				}
				return ;
			}
			last_val = minirt->controls.color_picker[2].btn.background_color;
			if (color_picker_action(&minirt->controls.color_picker[2], x, y))
			{
				if (last_val != minirt->controls.color_picker[2].btn.background_color)
				{
					minirt->micrort.sample = 0;
					stop_minirt(minirt);
				}
				return ;
			}
			if (text_input_focus(&minirt->controls.text_input[0], x, y))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[3]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[4]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[5]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[6]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[7]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[8]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[9]))
				return ;
			if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[10]))
				return ;
			if (float_input_focus(&minirt->controls.float_input[11], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[12], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[13], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[14], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[15], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[16], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[17], x, y))
				return ;
			if (float_input_focus(&minirt->controls.float_input[18], x, y))
				return ;
		}
	}
}

int	mousedown_controls(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	mousedown_common(key, minirt);
	layout_mouse_down(key, x, y, minirt);
	return (0);
}

int	mouseup_controls(int key, int x, int y, t_minirt *minirt)
{
	int	i;

	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	mouseup_common(key, minirt);
	i = -1;
	while (++i < minirt->controls.nb_buttons)
		button_release(&minirt->controls.buttons[i]);
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		slider_mouse_up(&minirt->controls.sliders[i]);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		color_picker_release(&minirt->controls.color_picker[i]);
	return (0);
}

int	mouse_move_controls(int x, int y, t_minirt *minirt)
{
	int	i;

	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		slider_mouse_move(&minirt->controls.sliders[i], x);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		color_picker_mouse_move(&minirt->controls.color_picker[i], x, y);	
	return (0);
}

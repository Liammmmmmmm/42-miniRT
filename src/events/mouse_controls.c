/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:41:18 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/25 17:38:09 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
		slider_mouse_down(&minirt->controls.sliders[1], x, y);
		slider_mouse_down(&minirt->controls.sliders[2], x, y);
		slider_mouse_down(&minirt->controls.sliders[3], x, y);
	}
	
	if (minirt->controls.ui_infos.tab_selected == 0)
	{
		if (mouse_down_obj(minirt, key, x, y))
			return ;
		if (minirt->controls.ui_infos.selected_object)
		{
			if (key == LEFT_CLICK)
			{
				int last_val = minirt->controls.color_picker[0].btn.background_color;
				if (color_picker_action(&minirt->controls.color_picker[0], x, y))
				{
					if (last_val != minirt->controls.color_picker[0].btn.background_color)
						stop_minirt(minirt);
					return ;
				}
			}
			if (mouse_down_dropdown(minirt, key, (t_point2){x, y}, &minirt->controls.dropdown[0]))
				return ;
		}
	}
	else if (minirt->controls.ui_infos.tab_selected == 1)
	{

		if (minirt->controls.ui_infos.selected_material)
		{
			
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

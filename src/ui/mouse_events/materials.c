/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:44:17 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 17:01:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "minirt.h"

int	material_tab_dropdowns2(int key, int x, int y, t_minirt *minirt)
{
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[8]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[9]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[10]))
		return (1);
	return (0);
}

int	material_tab_dropdowns(int key, int x, int y, t_minirt *minirt)
{
	if (color_picker_stop_minirt_mat(minirt, x, y,
		&minirt->controls.color_picker[1]))
		return (1);
	if (color_picker_stop_minirt_mat(minirt, x, y,
			&minirt->controls.color_picker[2]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[3]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[4]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[5]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[6]))
		return (1);
	if (mouse_down_dropdown_tex(minirt, key, (t_point2){x, y},
		&minirt->controls.dropdown[7]))
		return (1);
	return (0);
}

int	material_tab_events(int key, int x, int y, t_minirt *minirt)
{
	if (mouse_down_mat(minirt, key, x, y))
		return (1);
	if (key == LEFT_CLICK && x > 300 && x <= 600 && y > 270 && y < 300)
		add_material(minirt);
	if (minirt->controls.ui_infos.selected_material)
	{
		if (text_input_focus(&minirt->controls.text_input[0], x, y))
			return (1);
		if (material_tab_dropdowns(key, x, y, minirt)
			|| material_tab_dropdowns2(key, x, y, minirt))
			return (1);
		float_input_focus(&minirt->controls.float_input[11], x, y);
		float_input_focus(&minirt->controls.float_input[12], x, y);
		float_input_focus(&minirt->controls.float_input[13], x, y);
		float_input_focus(&minirt->controls.float_input[14], x, y);
		float_input_focus(&minirt->controls.float_input[15], x, y);
		float_input_focus(&minirt->controls.float_input[16], x, y);
		float_input_focus(&minirt->controls.float_input[17], x, y);
		float_input_focus(&minirt->controls.float_input[18], x, y);
	}
	return (0);
}

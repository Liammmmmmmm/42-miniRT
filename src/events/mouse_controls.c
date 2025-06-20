/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:41:18 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 15:02:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

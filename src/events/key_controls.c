/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_controls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:38:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/02 14:56:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	keydown_controls(int key, t_minirt *minirt)
{
	int	i;

	keydown_common(key, minirt);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		if (color_picker_type(&minirt->controls.color_picker[i], key))
			return (0);
	i = -1;
	while (++i < minirt->controls.nb_float_input)
	{
		if (float_input_type(&minirt->controls.float_input[i], key))
		{
			stop_minirt(minirt);
			if (key == 65293)
				minirt->micrort.sample = 0;
			if (minirt->controls.ui_infos.tab_selected == 0)
			minirt->scene.build_bvh = 1;
			return (0);
		}
	}
	if (minirt->controls.text_input[0].is_focused)
	{
		if (key == ' ')
			key = '_';
		if (!ft_isalnum(key) && key != '_' && key != 65288)
			return (0);
		if (text_input_type(&minirt->controls.text_input[0], key, minirt->controls.keydown.rshift || minirt->controls.keydown.lshift))
		{
			ft_strlcpy(minirt->controls.ui_infos.selected_material->name, minirt->controls.text_input[0].text, 21);
			stop_minirt(minirt);
			return (0);
		}
	}
	i = 0;
	while (++i < minirt->controls.nb_text_input)
	{
		if (text_input_type(&minirt->controls.text_input[i], key, minirt->controls.keydown.rshift || minirt->controls.keydown.lshift))
		{
			stop_minirt(minirt);
			return (0);
		}
	}
	return (0);
}

int	keyup_controls(int key, t_minirt *minirt)
{
	keyup_common(key, minirt);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_controls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:38:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 16:46:47 by madelvin         ###   ########.fr       */
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
		if (float_input_type(&minirt->controls.float_input[i], key))
			return (0);
	return (0);
}

int	keyup_controls(int key, t_minirt *minirt)
{
	keyup_common(key, minirt);
	return (0);
}

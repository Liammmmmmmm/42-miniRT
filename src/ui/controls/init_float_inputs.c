/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_float_inputs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:42:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 14:14:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_tiers_float_input(t_float_input *input)
{
	input->height = 20;
	input->width = 70;
	input->background_color = 0x0;
	input->border_color = UI_SEP_COLOR;
	input->border_color_focus = 0xFFFFFF;
	input->padding_left = 5;
	input->text_color = 0xFFFFFF;
}

int	init_float_inputs(t_minirt *minirt)
{
	minirt->controls.nb_float_input = 1;
	minirt->controls.float_input = ft_calloc(minirt->controls.nb_float_input,
		sizeof(t_float_input));
	if (!minirt->controls.float_input)
		return (0);
	// Object position
	base_tiers_float_input(&minirt->controls.float_input[0]);
	minirt->controls.float_input[0].x = 325;
	minirt->controls.float_input[0].y = 510;
	return (1);
}

void	clear_float_inputs(t_minirt *minirt)
{
	free(minirt->controls.float_input);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_float_inputs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:42:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/22 15:27:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_full_text_input(t_text_input *input)
{
	input->height = 30;
	input->width = 270;
	input->background_color = 0x0;
	input->border_color = UI_SEP_COLOR;
	input->border_color_focus = 0xFFFFFF;
	input->padding_left = 5;
	input->text_color = 0xFFFFFF;
	input->max_char = 20;
}

int	init_text_inputs(t_minirt *minirt)
{
	minirt->controls.nb_text_input = 11;
	minirt->controls.text_input = ft_calloc(minirt->controls.nb_text_input,
			sizeof(t_text_input));
	if (!minirt->controls.text_input)
		return (0);
	base_full_text_input(&minirt->controls.text_input[0]);
	minirt->controls.text_input[0].x = 315;
	minirt->controls.text_input[0].y = 310;
	ft_strlcpy(minirt->controls.text_input[0].placeholder, "Material name", 21);
	return (1);
}

void	clear_text_inputs(t_minirt *minirt)
{
	free(minirt->controls.text_input);
}

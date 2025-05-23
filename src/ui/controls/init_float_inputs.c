/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_float_inputs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:42:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 14:42:47 by lilefebv         ###   ########lyon.fr   */
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
	input->max_char = 9;
}

void	base_two_tiers_float_input(t_float_input *input)
{
	input->height = 20;
	input->width = 170;
	input->background_color = 0x0;
	input->border_color = UI_SEP_COLOR;
	input->border_color_focus = 0xFFFFFF;
	input->padding_left = 5;
	input->text_color = 0xFFFFFF;
	input->max_char = 9;
}

int	init_float_inputs(t_minirt *minirt)
{
	minirt->controls.nb_float_input = 11;
	minirt->controls.float_input = ft_calloc(minirt->controls.nb_float_input,
			sizeof(t_float_input));
	if (!minirt->controls.float_input)
		return (0);
	base_tiers_float_input(&minirt->controls.float_input[0]);
	minirt->controls.float_input[0].x = 325;
	minirt->controls.float_input[0].y = 510;
	base_tiers_float_input(&minirt->controls.float_input[1]);
	minirt->controls.float_input[1].x = 420;
	minirt->controls.float_input[1].y = 510;
	base_tiers_float_input(&minirt->controls.float_input[2]);
	minirt->controls.float_input[2].x = 520;
	minirt->controls.float_input[2].y = 510;
	base_tiers_float_input(&minirt->controls.float_input[3]);
	minirt->controls.float_input[3].x = 325;
	minirt->controls.float_input[3].y = 610;
	base_tiers_float_input(&minirt->controls.float_input[4]);
	minirt->controls.float_input[4].x = 420;
	minirt->controls.float_input[4].y = 610;
	base_tiers_float_input(&minirt->controls.float_input[5]);
	minirt->controls.float_input[5].x = 520;
	minirt->controls.float_input[5].y = 610;
	base_two_tiers_float_input(&minirt->controls.float_input[6]);
	minirt->controls.float_input[6].x = 400;
	minirt->controls.float_input[6].y = 905;
	base_two_tiers_float_input(&minirt->controls.float_input[7]);
	minirt->controls.float_input[7].x = 400;
	minirt->controls.float_input[7].y = 935;
	base_two_tiers_float_input(&minirt->controls.float_input[8]);
	minirt->controls.float_input[8].x = 400;
	minirt->controls.float_input[8].y = 965;
	base_two_tiers_float_input(&minirt->controls.float_input[9]);
	minirt->controls.float_input[9].x = 400;
	minirt->controls.float_input[9].y = 995;
	base_two_tiers_float_input(&minirt->controls.float_input[10]);
	minirt->controls.float_input[10].x = 400;
	minirt->controls.float_input[10].y = 1025;
	return (1);
}

void	clear_float_inputs(t_minirt *minirt)
{
	free(minirt->controls.float_input);
}

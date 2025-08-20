/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_float_inputs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:42:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/19 17:08:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_camera_float_inputs2(t_minirt *minirt)
{
	base_tiers_float_input(&minirt->controls.float_input[26]);
	minirt->controls.float_input[26].x = 160;
	minirt->controls.float_input[26].y = 244;
	minirt->controls.float_input[26].width = 130;
}

void	init_camera_float_inputs(t_minirt *minirt)
{
	base_tiers_float_input(&minirt->controls.float_input[19]);
	minirt->controls.float_input[19].x = 25;
	minirt->controls.float_input[19].y = 44;
	base_tiers_float_input(&minirt->controls.float_input[20]);
	minirt->controls.float_input[20].x = 120;
	minirt->controls.float_input[20].y = 44;
	base_tiers_float_input(&minirt->controls.float_input[21]);
	minirt->controls.float_input[21].x = 220;
	minirt->controls.float_input[21].y = 44;
	base_tiers_float_input(&minirt->controls.float_input[22]);
	minirt->controls.float_input[22].x = 25;
	minirt->controls.float_input[22].y = 114;
	base_tiers_float_input(&minirt->controls.float_input[23]);
	minirt->controls.float_input[23].x = 120;
	minirt->controls.float_input[23].y = 114;
	base_tiers_float_input(&minirt->controls.float_input[24]);
	minirt->controls.float_input[24].x = 220;
	minirt->controls.float_input[24].y = 114;
	if (!minirt->scene.camera.focus_dist)
		minirt->scene.camera.focus_dist = 1;
	base_tiers_float_input(&minirt->controls.float_input[25]);
	minirt->controls.float_input[25].x = 160;
	minirt->controls.float_input[25].y = 214;
	minirt->controls.float_input[25].width = 130;
	init_camera_float_inputs2(minirt);
}

static void	more_float_inputs(t_minirt *minirt)
{
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
}

int	init_float_inputs(t_minirt *minirt)
{
	minirt->controls.nb_float_input = 32;
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
	more_float_inputs(minirt);
	init_mat_float_input(minirt);
	init_camera_float_inputs(minirt);
	ambiant_and_more(minirt);
	minirt->controls.float_input[31].x = 100;
	minirt->controls.float_input[31].y = 574;
	minirt->controls.float_input[31].width = 190;
	link_float_input(&minirt->controls.float_input[31], &minirt->viewport.gamma)
	;
	return (1);
}

void	clear_float_inputs(t_minirt *minirt)
{
	free(minirt->controls.float_input);
}

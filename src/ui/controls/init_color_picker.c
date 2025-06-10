/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_color_picker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:55:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:39:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_cps(t_minirt *minirt)
{
	minirt->controls.nb_color_picker = 4;
	init_color_picker(&minirt->controls.color_picker[0],
		&minirt->controls.ui_infos.picker_no_color);
	minirt->controls.color_picker[0].btn.x = 310;
	minirt->controls.color_picker[0].btn.y = 710;
	init_color_picker(&minirt->controls.color_picker[1],
		&minirt->controls.ui_infos.picker_no_color);
	minirt->controls.color_picker[1].btn.x = 310;
	minirt->controls.color_picker[1].btn.y = 370;
	minirt->controls.color_picker[1].btn.width = 80;
	init_color_picker(&minirt->controls.color_picker[2],
		&minirt->controls.ui_infos.picker_no_color);
	minirt->controls.color_picker[2].btn.x = 310;
	minirt->controls.color_picker[2].btn.y = 670;
	minirt->controls.color_picker[2].btn.width = 80;
	init_color_picker(&minirt->controls.color_picker[3],
		&minirt->scene.amb_light.skybox_c);
	minirt->controls.color_picker[3].btn.x = 10;
	minirt->controls.color_picker[3].btn.y = 340;
	minirt->controls.color_picker[3].btn.width = 80;
	return (1);
}

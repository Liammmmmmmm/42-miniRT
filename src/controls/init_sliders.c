/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sliders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:13:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/26 18:25:27 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_slider(t_int_slider *slider)
{
	static int	height = 0;

	slider->x = 20;
	slider->y = 50 + height;
	slider->height = 3;
	slider->width = 260;
	slider->bar_color = 0xffffff;
	slider->point_color = 0x1193d9;
	height += 30;
}

void	init_focal_sliders(t_minirt *minirt)
{
	base_slider(&minirt->controls.sliders[0]);
	base_slider(&minirt->controls.sliders[1]);
	base_slider(&minirt->controls.sliders[2]);
	base_slider(&minirt->controls.sliders[3]);
	minirt->controls.values.focus_dist = 100;
	minirt->controls.sliders[0].min = 1;
	minirt->controls.sliders[0].max = 300;
	minirt->controls.sliders[0].value = &minirt->controls.values.focus_dist;
	minirt->controls.sliders[1].min = 0;
	minirt->controls.sliders[1].max	= 300;
	minirt->controls.sliders[1].value = &minirt->controls.values.defocus_angle;
	minirt->controls.values.fov = minirt->scene.camera.fov;
	minirt->controls.sliders[2].min = 0;
	minirt->controls.sliders[2].max	= 180;
	minirt->controls.sliders[2].value = &minirt->controls.values.fov;
	minirt->controls.values.gamma = 800;
	minirt->controls.sliders[3].min = 0;
	minirt->controls.sliders[3].max	= 1000;
	minirt->controls.sliders[3].value = &minirt->controls.values.gamma;
}

int	init_sliders(t_minirt *minirt)
{
	minirt->controls.nb_sliders = 4;
	minirt->controls.sliders = ft_calloc(minirt->controls.nb_sliders, sizeof(t_int_slider));
	if (!minirt->controls.sliders)
		return (0);
	init_focal_sliders(minirt);
	return (1);
}

void	clear_sliders(t_minirt *minirt)
{
	free(minirt->controls.sliders);
}

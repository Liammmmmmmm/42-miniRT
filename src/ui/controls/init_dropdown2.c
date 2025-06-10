/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dropdown2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:13:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:52:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_dropdown(t_dropdown *dropdown)
{
	dropdown->x = 310;
	dropdown->height = 20;
	dropdown->width = 280;
	dropdown->deployed_height = 180;
	dropdown->active = 0;
	dropdown->selected = NULL;
	dropdown->scroll_offset = 0;
}

void	base_dropdown_tex(t_dropdown *dropdown)
{
	dropdown->x = 400;
	dropdown->height = 20;
	dropdown->width = 190;
	dropdown->deployed_height = 180;
	dropdown->active = 0;
	dropdown->selected = NULL;
	dropdown->scroll_offset = 0;
}

void	init_tex_dropdown(t_minirt *minirt)
{
	base_dropdown_tex(&minirt->controls.dropdown[3]);
	base_dropdown_tex(&minirt->controls.dropdown[4]);
	base_dropdown_tex(&minirt->controls.dropdown[5]);
	base_dropdown_tex(&minirt->controls.dropdown[6]);
	base_dropdown_tex(&minirt->controls.dropdown[7]);
	base_dropdown_tex(&minirt->controls.dropdown[8]);
	base_dropdown_tex(&minirt->controls.dropdown[9]);
	base_dropdown_tex(&minirt->controls.dropdown[10]);
	minirt->controls.dropdown[3].y = 370;
	minirt->controls.dropdown[4].y = 420;
	minirt->controls.dropdown[5].y = 470;
	minirt->controls.dropdown[6].y = 570;
	minirt->controls.dropdown[7].y = 620;
	minirt->controls.dropdown[8].y = 670;
	minirt->controls.dropdown[9].y = 700;
	minirt->controls.dropdown[10].y = 800;
	base_dropdown_tex(&minirt->controls.dropdown[12]);
	minirt->controls.dropdown[12].y = 340;
	minirt->controls.dropdown[12].x = 100;
	minirt->controls.dropdown[12].selected
		= (void **)&minirt->scene.amb_light.skybox_t;
}

void	init_choose_object(t_minirt *minirt)
{
	base_dropdown(&minirt->controls.dropdown[11]);
	minirt->controls.dropdown[11].y = 340;
	minirt->controls.dropdown[11].selected
		= &minirt->controls.ui_infos.add_object;
	minirt->controls.dropdown[11].val_amount = 7;
	minirt->controls.dropdown[11].values = ft_calloc(
			minirt->controls.dropdown[11].val_amount, sizeof(t_dropdown_node));
	if (!minirt->controls.dropdown[11].values)
		return ;
	minirt->controls.dropdown[11].values[0].ref = (void *)SPHERE;
	minirt->controls.dropdown[11].values[0].text = get_object_name(SPHERE);
	minirt->controls.dropdown[11].values[1].ref = (void *)CYLINDER;
	minirt->controls.dropdown[11].values[1].text = get_object_name(CYLINDER);
	minirt->controls.dropdown[11].values[2].ref = (void *)CONE;
	minirt->controls.dropdown[11].values[2].text = get_object_name(CONE);
	minirt->controls.dropdown[11].values[3].ref = (void *)HYPERBOLOID;
	minirt->controls.dropdown[11].values[3].text = get_object_name(HYPERBOLOID);
	minirt->controls.dropdown[11].values[4].ref = (void *)PLANE;
	minirt->controls.dropdown[11].values[4].text = get_object_name(PLANE);
	minirt->controls.dropdown[11].values[5].ref = (void *)LIGHT;
	minirt->controls.dropdown[11].values[5].text = get_object_name(LIGHT);
	minirt->controls.dropdown[11].values[6].ref = (void *)DIRECTIONAL_LIGHT;
	minirt->controls.dropdown[11].values[6].text
		= get_object_name(DIRECTIONAL_LIGHT);
}

const char	*get_upscaling_name(t_upscalings up)
{
	static const char	*names[4] = {"Default", "Bilinear", "Bicubic"};

	return (names[(int)up]);
}

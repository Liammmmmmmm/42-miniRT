/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_tab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:12:34 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 13:02:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static inline void	draw_selected_object_prop(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	minirt->controls.font[0].size = 25;
	if (minirt->controls.ui_infos.selected_object->type == SPHERE)
		draw_sphere_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type == PLANE)
		draw_plane_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type == CYLINDER)
		draw_cyl_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type == LIGHT)
		draw_light_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type == CONE)
		draw_cone_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type == HYPERBOLOID)
		draw_hyperboloid_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type == CUSTOM)
		draw_custom_prop(img, minirt, ttf);
	else if (minirt->controls.ui_infos.selected_object->type
		== DIRECTIONAL_LIGHT)
		draw_directional_light_prop(img, minirt, ttf);
}

void	draw_objects_tab(t_img *img, t_minirt *minirt, t_ttf *ttf)
{
	draw_horizontal_line(img, 300, 300, img->width);
	draw_horizontal_line(img, 450, 300, img->width);
	draw_list_objects(minirt);
	if (minirt->controls.ui_infos.selected_object)
		draw_selected_object_prop(img, minirt, ttf);
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "Ajouter un objet", (t_point2){310, 325});
	minirt->controls.font[0].color = 0;
	display_button(img, minirt->controls.buttons[5], ttf);
	minirt->controls.font[0].color = 0xffffff;
	display_button(img, minirt->controls.buttons[6], ttf);
	display_dropdown(minirt, &minirt->controls.dropdown[11]);
}

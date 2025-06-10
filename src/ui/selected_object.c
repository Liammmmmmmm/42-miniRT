/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_object.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:45:58 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:19:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "ui.h"

void	set_selected_custom(t_minirt *minirt, t_custom_object *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->orientation.x);
	link_float_input(&minirt->controls.float_input[4], &obj->orientation.y);
	link_float_input(&minirt->controls.float_input[5], &obj->orientation.z);
	link_float_input(&minirt->controls.float_input[6], &obj->scale.x);
	link_float_input(&minirt->controls.float_input[7], &obj->scale.y);
	link_float_input(&minirt->controls.float_input[8], &obj->scale.z);
	minirt->controls.float_input[6].y = 905;
	minirt->controls.float_input[7].y = 935;
	minirt->controls.float_input[8].y = 965;
}

void	set_selected_light(t_minirt *minirt, t_light *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = NULL;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[6], &obj->brightness);
	minirt->controls.float_input[6].y = 905;
}

void	set_selected_directional_light(t_minirt *minirt, t_dlight *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = NULL;
	link_float_input(&minirt->controls.float_input[3], &obj->orientation.x);
	link_float_input(&minirt->controls.float_input[4], &obj->orientation.y);
	link_float_input(&minirt->controls.float_input[5], &obj->orientation.z);
	link_float_input(&minirt->controls.float_input[6], &obj->brightness);
	minirt->controls.float_input[6].y = 905;
}

void	set_selected_object(t_minirt *minirt, t_object *obj)
{
	if (!obj)
		return ;
	else if (obj->type == SPHERE)
		set_selected_sphere(minirt, obj->object);
	else if (obj->type == PLANE)
		set_selected_plane(minirt, obj->object);
	else if (obj->type == CYLINDER)
		set_selected_cylinder(minirt, obj->object);
	else if (obj->type == LIGHT)
		set_selected_light(minirt, obj->object);
	else if (obj->type == CONE)
		set_selected_cone(minirt, obj->object);
	else if (obj->type == HYPERBOLOID)
		set_selected_hyperboloid(minirt, obj->object);
	else if (obj->type == CUSTOM)
		set_selected_custom(minirt, obj->object);
	else if (obj->type == DIRECTIONAL_LIGHT)
		set_selected_directional_light(minirt, obj->object);
}

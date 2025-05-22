/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_object.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:45:58 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/22 11:57:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_selected_sphere(t_minirt *minirt, t_sphere *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[6], &obj->diameter);
	minirt->controls.float_input[6].y = 905;
}

void	set_selected_plane(t_minirt *minirt, t_plane *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->normal.x);
	link_float_input(&minirt->controls.float_input[4], &obj->normal.y);
	link_float_input(&minirt->controls.float_input[5], &obj->normal.z);
}

void	set_selected_cylinder(t_minirt *minirt, t_cylinder *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	minirt->controls.dropdown[1].selected = (void **)&obj->material_top;
	minirt->controls.dropdown[2].selected = (void **)&obj->material_bot;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->orientation.x);
	link_float_input(&minirt->controls.float_input[4], &obj->orientation.y);
	link_float_input(&minirt->controls.float_input[5], &obj->orientation.z);
	link_float_input(&minirt->controls.float_input[6], &obj->diameter);
	link_float_input(&minirt->controls.float_input[7], &obj->height);
	minirt->controls.float_input[6].y = 965;
	minirt->controls.float_input[7].y = 995;
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
	minirt->controls.float_input[7].y = 935;
}

void	set_selected_object(t_minirt *minirt, t_object *obj)
{
	if (!obj)
		return ;
	if (obj->type == SPHERE)
		set_selected_sphere(minirt, obj->object);
	if (obj->type == PLANE)
		set_selected_plane(minirt, obj->object);
	if (obj->type == CYLINDER)
		set_selected_cylinder(minirt, obj->object);
	if (obj->type == LIGHT)
		set_selected_light(minirt, obj->object);
}

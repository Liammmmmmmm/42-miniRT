/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_object.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:45:58 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 15:50:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_selected_sphere(t_minirt *minirt, t_sphere *obj)
{
	minirt->controls.color_picker[0].color = &obj->color;
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
}

void	set_selected_plane(t_minirt *minirt, t_plane *obj)
{
	minirt->controls.color_picker[0].color = &obj->color;
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
}

void	set_selected_cylinder(t_minirt *minirt, t_cylinder *obj)
{
	minirt->controls.color_picker[0].color = &obj->color;
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
}

void	set_selected_light(t_minirt *minirt, t_light *obj)
{
	minirt->controls.color_picker[0].color = &obj->color;
	minirt->controls.dropdown[0].selected = NULL;
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

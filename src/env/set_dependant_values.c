/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dependant_values.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:36:26 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 15:44:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_values_sphere(t_sphere *obj)
{
	obj->radius = obj->diameter / 2;
	obj->sqrt_radius = obj->radius * obj->radius;
}

void	set_values_cylinder(t_cylinder *obj)
{
	obj->orientation = vec3_unit(obj->orientation);
}

void	set_values_plane(t_plane *obj)
{
	obj->normal = vec3_unit(obj->normal);
}

void	set_values_light(t_light *obj)
{
	if (obj->brightness < 0)
		obj->brightness = 0;
}

void	set_values_cone(t_cone *obj)
{
	obj->orientation = vec3_unit(obj->orientation);
}

void	set_values_custom(t_custom_object *obj)
{
	obj->orientation = vec3_unit(obj->orientation);
}

void	set_values_hyperboloid(t_hyperboloid *obj)
{
	obj->orientation = vec3_unit(obj->orientation);
}

void	set_dependant_values(t_minirt *minirt)
{
	int	i;
	
	if (minirt->controls.ui_infos.selected_object == NULL)
		return ;
	i = -1;
	while (++i < minirt->scene.el_amount)
	{
		if (minirt->controls.ui_infos.tab_selected == 0)
		{
			if (minirt->controls.ui_infos.selected_object[i].type == SPHERE)
				set_values_sphere(minirt->controls.ui_infos.selected_object[i].object);
			else if (minirt->controls.ui_infos.selected_object[i].type == CYLINDER)
				set_values_cylinder(minirt->controls.ui_infos.selected_object[i].object);
			else if (minirt->controls.ui_infos.selected_object[i].type == PLANE)
				set_values_plane(minirt->controls.ui_infos.selected_object[i].object);
			else if (minirt->controls.ui_infos.selected_object[i].type == LIGHT)
				set_values_light(minirt->controls.ui_infos.selected_object[i].object);
			else if (minirt->controls.ui_infos.selected_object[i].type == CONE)
				set_values_cone(minirt->controls.ui_infos.selected_object[i].object);
			else if (minirt->controls.ui_infos.selected_object[i].type == HYPERBOLOID)
				set_values_hyperboloid(minirt->controls.ui_infos.selected_object[i].object);
		else if (minirt->controls.ui_infos.selected_object[i].type == CUSTOM)
			set_values_custom(minirt->controls.ui_infos.selected_object[i].object);
		}
		else if (minirt->controls.ui_infos.tab_selected == 1)
		{

		}
	}
}

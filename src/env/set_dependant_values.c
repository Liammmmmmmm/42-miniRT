/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dependant_values.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:36:26 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/22 12:28:52 by lilefebv         ###   ########lyon.fr   */
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

void	set_dependant_values(t_minirt *minirt)
{
	int	i;
	
	i = -1;
	while (++i < minirt->scene.el_amount)
	{
		if (minirt->controls.ui_infos.selected_object[i].type == SPHERE)
			set_values_sphere(minirt->controls.ui_infos.selected_object[i].object);
		else if (minirt->controls.ui_infos.selected_object[i].type == CYLINDER)
			set_values_cylinder(minirt->controls.ui_infos.selected_object[i].object);
		else if (minirt->controls.ui_infos.selected_object[i].type == PLANE)
			set_values_plane(minirt->controls.ui_infos.selected_object[i].object);
		else if (minirt->controls.ui_infos.selected_object[i].type == LIGHT)
			set_values_light(minirt->controls.ui_infos.selected_object[i].object);
	}
}

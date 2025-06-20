/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dependant_values_objs.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:36:26 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 15:11:11 by lilefebv         ###   ########lyon.fr   */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dependant_values.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:36:26 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/04 15:54:11 by lilefebv         ###   ########lyon.fr   */
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

void	set_values_directional_light(t_dlight *obj)
{
	obj->orientation = vec3_unit(obj->orientation);
}

void	set_values_mat(t_mat *mat)
{
	mat->emission_color = color_to_fcolor(mat->emission_color_tmp);
	mat->metallic_value = clamp_double(mat->metallic_value);
	mat->roughness_value = clamp_double(mat->roughness_value);
}

void	set_dependant_values(t_minirt *minirt)
{
	int	i;
	
	i = -1;
	while (++i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
			set_values_sphere(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == CYLINDER)
			set_values_cylinder(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == PLANE)
			set_values_plane(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == LIGHT)
			set_values_light(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == CONE)
			set_values_cone(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == HYPERBOLOID)
			set_values_hyperboloid(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == CUSTOM)
			set_values_custom(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == DIRECTIONAL_LIGHT)
			set_values_directional_light(minirt->scene.elements[i].object);
	}
	i = -1;
	while (++i < minirt->scene.mat_amount)
		set_values_mat(&minirt->scene.materials[i]);
}

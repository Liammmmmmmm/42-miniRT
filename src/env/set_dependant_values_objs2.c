/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dependant_values_objs2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:36:26 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 15:13:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

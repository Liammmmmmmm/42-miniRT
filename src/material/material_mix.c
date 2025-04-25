/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_mix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:28:45 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/24 12:23:19 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

// t_color	material_mix(t_mat_manager *mat_man)
// {
// 	if (mat_man->hit_record.mat->metallic_value > 0.0)
// 		return (reflection_color(mat_man));
// 	else if (mat_man->hit_record.mat->ior > 0 && mat_man->hit_record.mat->transmission > 0)
// 		return (refracted_color(mat_man));
// 	else
// 		return (material_default(mat_man));
// 	return ((t_color){0, 0, 0});
// }

// t_color	color_lerp(t_color a, t_color b, float t)
// {
// 	return ((t_color) {
// 		.r = a.r + t * ((int16_t)b.r - a.r),
// 		.g = a.g + t * ((int16_t)b.g - a.g),
// 		.b = a.b + t * ((int16_t)b.b - a.b)
// 	});
// }

t_ray_data	dielectric_material(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.mat->ior > 0)
	{
		if (mat_man->hit_record.mat->transmission == 1.0)
			return (dielectric_transmissive_material(mat_man));
		else if (mat_man->hit_record.mat->transmission == 0.0)
			return (dielectric_non_transmissive_material(mat_man));
		else
		{
			if (random_double() > mat_man->hit_record.mat->transmission)
				return (dielectric_non_transmissive_material(mat_man));
			else
				return (dielectric_transmissive_material(mat_man));
		}
	}
	else
		return (material_default(mat_man));
}

t_ray_data	material_mix_v2(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.mat->emission_strength > 0.0)
		return ((t_ray_data){emissive_material(mat_man), EMISSIVE});
	else
	{
		
		if (mat_man->hit_record.mat->metallic_value == 1.0)
			return (metallic_material(mat_man));
		else if (mat_man->hit_record.mat->metallic_value == 0.0)
			return (dielectric_material(mat_man));
		else
		{
			if (random_double() > mat_man->hit_record.mat->metallic_value)
				return (dielectric_material(mat_man));
			else
				return (metallic_material(mat_man));
		}
	}
	return ((t_ray_data){(t_color){0, 0, 0}, DEFFAULT});
}


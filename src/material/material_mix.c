/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_mix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:28:45 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 16:39:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

t_color	material_mix(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.mat->metallic_value > 0.0)
		return (reflection_color(mat_man));
	else if (mat_man->hit_record.mat->ior > 0 && mat_man->hit_record.mat->transmission > 0)
		return (refracted_color(mat_man));
	else
		return (material_default(mat_man));
	return ((t_color){0, 0, 0});
}

t_color	color_lerp(t_color a, t_color b, float t)
{
	return ((t_color) {
		.r = a.r + t * ((int16_t)b.r - a.r),
		.g = a.g + t * ((int16_t)b.g - a.g),
		.b = a.b + t * ((int16_t)b.b - a.b)
	});
}

t_color	dielectric_material(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.mat->ior > 0)
	{
		if (mat_man->hit_record.mat->transmission > 0)
			return (dielectric_transmissive_material(mat_man));
		else
			return (dielectric_non_transmissive_material(mat_man));
	}
	else
		return (material_default(mat_man));
}

t_color	material_mix_v2(t_mat_manager *mat_man)
{
	// if (mat_man->hit_record.color.r != 0
	// 	|| mat_man->hit_record.color.g != 0
	// 	|| mat_man->hit_record.color.b != 0)
	// { // faut pas tester si le rayon est noir ici, sinon ca peut peter des trucs
		if (mat_man->hit_record.mat->metallic_value == 1.0)
			return (metallic_material(mat_man));
		else if (mat_man->hit_record.mat->metallic_value == 0.0)
			return (dielectric_material(mat_man));
		else
		{
			// return (color_lerp(dielectric_material(mat_man),
			// 	metallic_material(mat_man),
			// 	mat_man->hit_record.mat->metallic_value));
			// Idealement remplacer ca par un dielectric_material() * (1 - metallic) + metallic_material() * metallic
			// Encore mieux : la russian roulette
			if (random_double() > mat_man->hit_record.mat->metallic_value)
				return (dielectric_material(mat_man));
			else
				return (metallic_material(mat_man));
		}
	// }
	return ((t_color){0, 0, 0});
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_mix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:28:45 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/03 19:17:06 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

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

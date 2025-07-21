/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_photon_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:56:26 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/21 14:44:58 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "structs.h"
#include "material.h"
#include "maths.h"

char	material_manager_v3_phothon(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_fcolor *power)
{
	char	return_value;

	return_value = 0;
	if (!hit_record->mat)
		return (default_mat_photon(hit_record, power));
	if (hit_record->mat->metallic_value == 1.0)
		metallic_color(ray, hit_record, power);
	else if (hit_record->mat->metallic_value == 0.0)
		return_value += dielectric_mat_photon(minirt, ray, hit_record, power);
	else if (hit_record->mat->metallic_value > random_double())
		metallic_color(ray, hit_record, power);
	else
		return_value += dielectric_mat_photon(minirt, ray, hit_record, power);
	return (return_value > 0);
}

void	init_single_photon_path(t_ray *ray, t_vec3 light_position,
	t_hit_register_data *data, int *depth)
{
	ray->dir = vec3_random_unit();
	ray->orig = light_position;
	data->is_light = 1;
	data->ray = ray;
	*depth = 0;
}

char	trace_single_photon_path(t_minirt *minirt, t_vec3 light_position,
	t_fcolor initial_power, t_photon *photon_out)
{
	t_ray				ray;
	t_fcolor			photon_power;
	t_hit_register_data	data;
	int					depth;

	init_single_photon_path(&ray, light_position, &data, &depth);
	photon_power = initial_power;
	while (depth < 10)
	{
		if (hit_register_all(minirt, &data) == 1)
		{
			ray.orig = data.hit_record.point;
			if (material_manager_v3_phothon(minirt, &ray, &data.hit_record,
					&photon_power))
			{
				photon_out->position = data.hit_record.point;
				photon_out->power = photon_power;
				return (depth != 0);
			}
		}
		else
			return (0);
		depth++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_photon_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:56:26 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/30 15:55:17 by madelvin         ###   ########.fr       */
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

char	trace_single_photon_path(t_minirt *minirt, t_vec3 light_position,
	t_fcolor initial_power, t_photon *photon_out)
{
    t_ray               ray;
    t_fcolor            photon_power;
    t_hit_register_data data;
    int                 depth;

    ray.dir = vec3_random_unit();
	ray.orig = light_position;
    photon_power = initial_power;
    data.is_light = 1;
    data.ray = &ray;
    depth = 0;
	photon_out->i = 0;
	photon_out->initial = light_position;
    while (depth < 10)
    {
        if (hit_register_all(minirt, &data) == 1)
        {
			ray.orig = data.hit_record.point;
			if (material_manager_v3_phothon(minirt, &ray, &data.hit_record,
				&photon_power))
			{
				if (depth == 0)
					photon_out->p0 = data.hit_record.point;
				if (depth == 1)
					photon_out->p1 = data.hit_record.point;
				if (depth == 2)
					photon_out->p2 = data.hit_record.point;
				if (depth == 3)
					photon_out->p3 = data.hit_record.point;
				if (depth == 4)
					photon_out->p4 = data.hit_record.point;
				if (depth == 5)
					photon_out->p5 = data.hit_record.point;
				if (depth == 6)
					photon_out->p6 = data.hit_record.point;
				if (depth == 7)
					photon_out->p7 = data.hit_record.point;
				if (depth == 8)
					photon_out->p8 = data.hit_record.point;
				if (depth == 9)
					photon_out->p9 = data.hit_record.point;
				if (depth == 10)
					photon_out->p10 = data.hit_record.point;
				photon_out->i = depth;
				photon_out->position = data.hit_record.point;
				photon_out->power = photon_power;
				return (1);
			}
        }
        else
			return (0);
		if (depth == 0)
			photon_out->p0 = data.hit_record.point;
		if (depth == 1)
			photon_out->p1 = data.hit_record.point;
		if (depth == 2)
			photon_out->p2 = data.hit_record.point;
		if (depth == 3)
			photon_out->p3 = data.hit_record.point;
		if (depth == 4)
			photon_out->p4 = data.hit_record.point;
		if (depth == 5)
			photon_out->p5 = data.hit_record.point;
		if (depth == 6)
			photon_out->p6 = data.hit_record.point;
		if (depth == 7)
			photon_out->p7 = data.hit_record.point;
		if (depth == 8)
			photon_out->p8 = data.hit_record.point;
		if (depth == 9)
			photon_out->p9 = data.hit_record.point;
		if (depth == 10)
			photon_out->p10 = data.hit_record.point;
        depth++;
    }
	return (0);
}

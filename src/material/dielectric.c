/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:30:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/14 15:47:02 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

static inline void	reflected_dielectric_color(t_ray *ray,
	t_hit_record *hit_record)
{
	t_vec3	direction;
	t_vec3	micro_normal;

	if (hit_record->mat->roughness_value > 0.0)
		micro_normal = ggx_sample_hemisphere(hit_record->normal,
				hit_record->mat->roughness_value);
	else
		micro_normal = hit_record->normal;
	direction = vec3_subtract(
			ray->dir,
			vec3_multiply_scalar(
				micro_normal,
				2 * vec3_dot(ray->dir, micro_normal)
				)
			);
	ray->dir = vec3_unit(direction);
}

// In order to implement specular map just multiply 
// get_reflect_value(ray, hit_record) by the specular value
//
inline void	dielectric_mat(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data data)
{
	if (hit_record->mat->ior > 0)
	{
		if (get_reflect_value(ray, hit_record) >= random_double())
			reflected_dielectric_color(ray, hit_record);
		else
		{
			if (hit_record->mat->transmission == 1.0)
				refracted_ray(minirt, ray, hit_record, data.power);
			else if (hit_record->mat->transmission == 0.0)
				default_mat(minirt, ray, hit_record, data);
			else
			{
				if (hit_record->mat->transmission < random_double())
					default_mat(minirt, ray, hit_record, data);
				else
					refracted_ray(minirt, ray, hit_record, data.power);
			}
		}
	}
	else
		default_mat(minirt, ray, hit_record, data);
}

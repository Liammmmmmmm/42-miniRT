/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:34:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 19:14:47 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

double	get_cos_theta(t_vec3 dir_in, t_vec3 normal)
{
	return (-vec3_dot(vec3_unit(dir_in), normal));
}

t_vec3	reflection_vec(const t_vec3 uv, const t_vec3 n)
{
	t_vec3	direction;

	direction = vec3_subtract(uv, vec3_multiply_scalar(n, 2 * vec3_dot(uv, n)));
	return (direction);
}

inline double	get_reflect_value(t_ray *ray_in, t_hit_record *hit_record)
{
	return (fresnel_schlick_f(
			get_cos_theta(ray_in->dir, hit_record->normal),
			powf((hit_record->mat->ior - 1)
				/ (hit_record->mat->ior + 1), 2)
		)
	);
}

inline t_vec3	ggx_sample_hemisphere(t_vec3 normal, double roughness,
	uint64_t *rand)
{
	t_vec3	tangent;
	double	u2;
	double	phi;
	double	cos_theta;
	double	sin_theta;

	u2 = random_double(rand);
	phi = 2.0 * PI_D * random_double(rand);
	cos_theta = sqrt((1.0 - u2) / (1.0 + ((roughness * roughness) * \
	(roughness * roughness) - 1.0) * u2));
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	tangent = vec3_unit(vec3_cross(normal, (t_vec3){1.0, 0.0, 0.0}));
	if (vec3_length_squared(tangent) < 0.0001)
		tangent = vec3_unit(vec3_cross(normal, (t_vec3){0.0, 1.0, 0.0}));
	return (vec3_add(
			vec3_multiply_scalar(normal, cos_theta),
			vec3_add(
				vec3_multiply_scalar(tangent, cos(phi) * sin_theta),
				vec3_multiply_scalar((vec3_unit(vec3_cross(normal, tangent))),
					sin(phi) * sin_theta)
			)
		));
}

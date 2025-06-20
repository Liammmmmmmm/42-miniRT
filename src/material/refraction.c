/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:33:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 18:56:31 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

static inline t_vec3	refracted_v(const t_vec3 uv, const t_vec3 n, double ri)
{
	t_vec3	r_out_perp;

	r_out_perp = vec3_multiply_scalar(
			vec3_add(
				uv,
				vec3_multiply_scalar(
					n,
					fmin(vec3_dot(vec3_negate(uv), n), 1.0)
					)
				),
			ri
			);
	return (
		vec3_add(
			vec3_multiply_scalar(
				n,
				-sqrt(fabs(1.0 - vec3_length_squared(r_out_perp)))
			),
			r_out_perp
		)
	);
}

static inline void	refracted_value(t_ray *ray, t_hit_record *hit_record,
	t_fcolor *power, double eta)
{
	t_vec3		direction;

	direction = refracted_v(
			ray->dir,
			hit_record->normal, eta
			);
	direction = vec3_unit(direction);
	if (hit_record->mat->roughness_value > 0.0)
		direction = ggx_sample_hemisphere(direction, \
				hit_record->mat->roughness_value);
	ray->dir = direction;
	*power = multiply_fcolor(*power, hit_record->color);
}

inline void	refracted_ray(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_fcolor *power)
{
	double		eta;
	double		cos_theta;
	double		sin_theta;

	if (hit_record->front_face)
		eta = minirt->scene.ior_all / hit_record->mat->ior;
	else
		eta = hit_record->mat->ior / minirt->scene.ior_all;
	ray->dir = vec3_unit(ray->dir);
	cos_theta = fmin(-vec3_dot(ray->dir, hit_record->normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	if (eta * sin_theta > 1.0)
		metallic_color(ray, hit_record, power);
	else
		refracted_value(ray, hit_record, power, eta);
}

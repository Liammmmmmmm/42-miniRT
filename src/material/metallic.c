/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metallic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:42:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/14 10:43:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

/**
 * Calc color of a ray hitting a metallic surface
 * 
 * direction : bouncing ray
 * `R = I−2(I⋅N)N`
 * with
 * • `I` : the direction of the ray hitting the object
 * • `N` : normal of the point
 */
inline void	metallic_color(t_ray *ray, t_hit_record	*hit_record,
	t_fcolor *power)
{
	t_vec3			direction;
	const double	cos_theta = get_cos_theta(ray->dir, hit_record->normal);

	direction = vec3_subtract(
			ray->dir,
			vec3_multiply_scalar(hit_record->normal,
				2 * vec3_dot(ray->dir, hit_record->normal)
				)
			);
	direction = vec3_unit(direction);
	if (hit_record->mat->roughness_value > 0.0)
			direction = ggx_sample_hemisphere(hit_record->normal, \
				hit_record->mat->roughness_value);
	ray->dir = direction;
	*power = multiply_fcolor(
			*power,
			fresnel_schlick_color(cos_theta, hit_record->color)
			);
}

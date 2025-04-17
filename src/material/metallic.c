/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metallic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:42:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/16 13:50:53 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * Calc color of a ray hitting a metallic surface
 * 
 * direction : bouncing ray
 * `R = I−2(I⋅N)N`
 * with
 * • `I` : the direction of the ray hitting the object
 * • `N` : normal of the point
 */
t_color	metallic_material(t_mat_manager *mat_man)
{
	t_vec3	direction;
	t_color	bounce_color;
	const double	cos_theta = get_cos_theta(mat_man->ray_in.dir, mat_man->hit_record.normal);

	direction = vec3_subtract(mat_man->ray_in.dir, vec3_multiply_scalar(mat_man->hit_record.normal, 2 * vec3_dot(mat_man->ray_in.dir, mat_man->hit_record.normal)));
	if (mat_man->hit_record.mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction),
			vec3_multiply_scalar(vec3_random_unit(),
			mat_man->hit_record.mat->roughness_value));
	mat_man->ray_in.dir = vec3_unit(direction);
	mat_man->ray_in.orig = mat_man->hit_record.point;

	bounce_color = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, NULL);

	return (color_multiply(
		fresnel_schlick_color(cos_theta, mat_man->color),
		bounce_color)
	);
}

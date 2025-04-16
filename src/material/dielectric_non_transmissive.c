/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_non_transmissive.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:34 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/16 13:51:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	dielectric_non_transmissive_material(t_mat_manager *mat_man)
{
	t_vec3			direction;
	t_color			specular_bounce;
	t_color			diffuse_bounce;
	const double	f = fresnel_schlick_f(get_cos_theta(mat_man->ray_in.dir, mat_man->hit_record.normal), 0.04);

	direction = vec3_subtract(mat_man->ray_in.dir, vec3_multiply_scalar(mat_man->hit_record.normal, 2 * vec3_dot(mat_man->ray_in.dir, mat_man->hit_record.normal)));
	if (mat_man->hit_record.mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction),
			vec3_multiply_scalar(vec3_random_unit(),
			mat_man->hit_record.mat->roughness_value));
	mat_man->ray_in.dir = vec3_unit(direction);
	mat_man->ray_in.orig = mat_man->hit_record.point;

	int max_bounce = mat_man->depth - 1;
	// if (max_bounce > 3)
	// 	max_bounce = 3;
	specular_bounce = ray_color(mat_man->minirt, mat_man->ray_in, max_bounce, NULL);

	direction = vec3_add(mat_man->hit_record.normal, vec3_random_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
		direction = mat_man->hit_record.normal;
	mat_man->ray_in.dir = direction;

	diffuse_bounce = ray_color(mat_man->minirt, mat_man->ray_in, max_bounce, NULL);

	return (color_add(
		color_scale(specular_bounce, f),
		color_scale(color_multiply(mat_man->color, diffuse_bounce), 1 - f)
	));
}

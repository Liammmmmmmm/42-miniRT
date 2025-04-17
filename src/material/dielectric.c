/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:30:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/17 15:35:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

inline t_color	reflected_dielectric_color(t_mat_manager *mat_man)
{
	t_vec3	direction;

	direction = vec3_subtract(mat_man->ray_in.dir, vec3_multiply_scalar(mat_man->hit_record.normal, 2 * vec3_dot(mat_man->ray_in.dir, mat_man->hit_record.normal)));
	if (mat_man->hit_record.mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction), vec3_multiply_scalar(\
			vec3_random_unit(), mat_man->hit_record.mat->roughness_value));
	mat_man->ray_in.dir = vec3_unit(direction);
	mat_man->ray_in.orig = mat_man->hit_record.point;
	return (ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, NULL));
}

inline double	get_reflect_value(t_mat_manager *mat_man)
{
	return (fresnel_schlick_f(
		get_cos_theta(mat_man->ray_in.dir, mat_man->hit_record.normal),
		powf(
			(mat_man->hit_record.mat->ior - 1) / (mat_man->hit_record.mat->ior + 1),
			2)
		)
	);
}

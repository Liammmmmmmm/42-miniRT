/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_transmissive.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:50 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/17 15:54:53 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline t_color	refracted_ray(t_mat_manager *mat_man)
{
	double	eta;
	double	cos_theta;
	double	sin_theta;
	t_vec3	direction;
	t_color	bounce_color;

	if (mat_man->hit_record.front_face)
		eta = mat_man->minirt->scene.ior_all / mat_man->hit_record.mat->ior;
	else
		eta = mat_man->hit_record.mat->ior / mat_man->minirt->scene.ior_all;
	mat_man->ray_in.dir = vec3_unit(mat_man->ray_in.dir);

	cos_theta = fmin(-vec3_dot(mat_man->ray_in.dir, mat_man->hit_record.normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	if (eta * sin_theta > 1.0)
		return (metallic_material(mat_man));
	else
	{
		direction = refracted_vec(mat_man->ray_in.dir, mat_man->hit_record.normal, eta);
		if (mat_man->hit_record.mat->roughness_value > 0.0)
			direction = vec3_add(vec3_unit(direction),
				vec3_multiply_scalar(vec3_random_unit(),
				mat_man->hit_record.mat->roughness_value));
		mat_man->ray_in.dir = vec3_unit(direction);
		mat_man->ray_in.orig = mat_man->hit_record.point;

		bounce_color = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, NULL);
		return (color_multiply(mat_man->hit_record.color, bounce_color)
		);
	}
}

t_color	dielectric_transmissive_material(t_mat_manager *mat_man)
{
	const double	f = get_reflect_value(mat_man);

	if (f >= random_double())
		return (reflected_dielectric_color(mat_man));
	else
		return (refracted_ray(mat_man));
}

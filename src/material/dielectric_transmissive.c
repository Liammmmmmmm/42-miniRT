/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_transmissive.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:50 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/24 12:25:27 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline t_vec3	refracted_v(const t_vec3 uv, const t_vec3 n, double ri)
{
	t_vec3	r_out_perp;

	r_out_perp = vec3_multiply_scalar(vec3_add(uv, \
		vec3_multiply_scalar(n, fmin(vec3_dot(vec3_negate(uv), n), 1.0))), ri);
	return (vec3_add(vec3_multiply_scalar(n, \
		-sqrt(fabs(1.0 - vec3_length_squared(r_out_perp)))), r_out_perp));
}

static inline t_ray_data	refracted_ray(t_mat_manager *mat_man)
{
	double		eta;
	double		cos_theta;
	double		sin_theta;
	t_vec3		direction;
	t_ray_data	ray_data;

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
		direction = refracted_v(mat_man->ray_in.dir, mat_man->hit_record.normal, eta);
		if (mat_man->hit_record.mat->roughness_value > 0.0)
			direction = vec3_add(vec3_unit(direction),
				vec3_multiply_scalar(vec3_random_unit(),
				mat_man->hit_record.mat->roughness_value));
		mat_man->ray_in.dir = vec3_unit(direction);
		mat_man->ray_in.orig = mat_man->hit_record.point;

		ray_data = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, NULL);
		return ((t_ray_data){color_multiply(mat_man->hit_record.color, ray_data.color), ray_data.mat_type});
	}
}

t_ray_data	dielectric_transmissive_material(t_mat_manager *mat_man)
{
	const double	f = get_reflect_value(mat_man);

	if (f >= random_double())
		return (reflected_dielectric_color(mat_man));
	else
		return (refracted_ray(mat_man));
}

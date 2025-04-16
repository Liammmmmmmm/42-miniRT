/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_non_transmissive.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:34 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/16 16:15:27 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	russian_roulette_dielectric(t_mat_manager *mat_man, t_color *bounce)
{
	t_vec3			direction;
	char			bounce_hit;
	const double	f = fresnel_schlick_f(get_cos_theta(\
	mat_man->ray_in.dir, mat_man->hit_record.normal), powf((\
	mat_man->hit_record.mat->ior - 1) / (mat_man->hit_record.mat->ior + 1), 2));

	if (f >= random_double())
	{
		direction = vec3_subtract(mat_man->ray_in.dir, vec3_multiply_scalar(mat_man->hit_record.normal, 2 * vec3_dot(mat_man->ray_in.dir, mat_man->hit_record.normal)));
		if (mat_man->hit_record.mat->roughness_value > 0.0)
			direction = vec3_add(vec3_unit(direction), vec3_multiply_scalar(\
				vec3_random_unit(), mat_man->hit_record.mat->roughness_value));
		mat_man->ray_in.dir = vec3_unit(direction);
		mat_man->ray_in.orig = mat_man->hit_record.point;
		*bounce = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, NULL);
	}
	else
	{
		bounce_hit = 0;
		direction = vec3_add(mat_man->hit_record.normal, vec3_random_unit());
		if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
			direction = mat_man->hit_record.normal;
		mat_man->ray_in.dir = direction;
		*bounce = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, &bounce_hit);
		if (bounce_hit)
			*bounce = color_multiply(mat_man->color, *bounce);
		else
		{
			*bounce = mat_man->color;
			// *bounce = color_lerp(mat_man->color, *bounce, 0.4); // Pour avoir la skybox qui influe legerement
		}
	}
}

t_color	dielectric_non_transmissive_material(t_mat_manager *mat_man)
{
	t_color			bounce;

	russian_roulette_dielectric(mat_man, &bounce);
	return (bounce);
}

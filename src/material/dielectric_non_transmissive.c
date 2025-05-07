/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_non_transmissive.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:34 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/06 10:51:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline t_ray_data	random_bounce_ray(t_mat_manager *mat_man)
{
	t_vec3			direction;
	char			bounce_hit;
	t_ray_data		ray_data;

	bounce_hit = 0;
	direction = vec3_add(mat_man->hit_record.normal, vec3_random_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && \
			fabs(direction.z) < 1e-8)
		direction = mat_man->hit_record.normal;
	mat_man->ray_in.dir = direction;
	ray_data = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, \
		&bounce_hit);
	// if (bounce_hit)
	// {
		if (ray_data.mat_type == DEFFAULT)
			return ((t_ray_data){color_multiply(mat_man->color, \
				ray_data.color), ray_data.mat_type});
		else
			return ((t_ray_data){color_add_clamp(ray_data.color, \
				color_multiply(mat_man->color, ray_data.color)), \
				ray_data.mat_type});
	// }
	// else
	// 	return ((t_ray_data){mat_man->color, ray_data.mat_type});
}

t_ray_data	dielectric_non_transmissive_material(t_mat_manager *mat_man)
{
	const double	f = get_reflect_value(mat_man);

	if (f >= random_double())
		return (reflected_dielectric_color(mat_man));
	else
		return (random_bounce_ray(mat_man));
}

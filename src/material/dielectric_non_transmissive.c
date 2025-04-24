/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_non_transmissive.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:34 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/24 11:18:35 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline t_color	random_bounce_ray(t_mat_manager *mat_man)
{
	t_vec3			direction;
	char			bounce_hit;
	t_ray_data		ray_data;

	bounce_hit = 0;
	direction = vec3_add(mat_man->hit_record.normal, vec3_random_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
		direction = mat_man->hit_record.normal;
	mat_man->ray_in.dir = direction;
	ray_data = ray_color(mat_man->minirt, mat_man->ray_in, mat_man->depth - 1, &bounce_hit);
	if (bounce_hit)
		return (color_multiply(mat_man->color, ray_data.color));
	else
	{
		return (mat_man->color);
		// *bounce = color_lerp(mat_man->color, *bounce, 0.4); // Pour avoir la skybox qui influe legerement
	}
}

t_color	dielectric_non_transmissive_material(t_mat_manager *mat_man)
{
	const double	f = get_reflect_value(mat_man);

	if (f >= random_double())
		return (reflected_dielectric_color(mat_man));
	else
		return (random_bounce_ray(mat_man));
}

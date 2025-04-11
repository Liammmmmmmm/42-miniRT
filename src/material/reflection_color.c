/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:26:22 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 19:52:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "maths.h"

t_color	reflection_color(t_mat_manager *mat_man)
{
	t_vec3	direction;
	t_color	bounce_color;

	direction = vec3_subtract(mat_man->ray_in.dir, \
		vec3_multiply_scalar(mat_man->hit_record.normal, \
		2 * vec3_dot(mat_man->ray_in.dir, mat_man->hit_record.normal)));
	if (mat_man->hit_record.mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction), \
			vec3_multiply_scalar(vec3_random_unit(), \
			mat_man->hit_record.mat->roughness_value));
	mat_man->ray_in.dir = direction;
	mat_man->ray_in.orig = mat_man->hit_record.point;
	bounce_color = ray_color(mat_man->minirt, mat_man->ray_in, \
					mat_man->depth - 1, NULL);
	bounce_color = color_scale(bounce_color, \
					mat_man->hit_record.mat->metallic_value);
	mat_man->color = color_scale(mat_man->color, \
					1 - mat_man->hit_record.mat->metallic_value);
	mat_man->color = color_add(mat_man->color, bounce_color);
	return (mat_man->color);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/14 10:35:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "maths.h"

t_color	material_default(t_mat_manager *mat_man)
{
	t_vec3	direction;
	t_ray	ray;
	char	bounce_hit;
	t_color	bounce_color;

	bounce_hit = 0;
	direction = vec3_add(mat_man->hit_record.normal, vec3_random_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && \
												fabs(direction.z) < 1e-8)
		direction = mat_man->hit_record.normal;
	ray.dir = direction;
	ray.orig = mat_man->hit_record.point;
	bounce_color = ray_color(mat_man->minirt, ray, mat_man->depth - 1, \
												&bounce_hit);
	if (bounce_hit)
		mat_man->color = color_multiply(mat_man->color, bounce_color);
	return (mat_man->color);
}

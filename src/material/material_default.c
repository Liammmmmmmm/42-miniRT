/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/09 16:19:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "material.h"

inline void	default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_ray_data data)
{
	ray->dir = vec3_add(hit_record->normal, vec3_random_unit());
	if (fabs(ray->dir.x) < 1e-8 && fabs(ray->dir.y) < 1e-8
		&& fabs(ray->dir.z) < 1e-8)
		ray->dir = hit_record->normal;
	*data.accumulation = add_fcolor(
			*data.accumulation,
			multiply_fcolor(
				multiply_fcolor(
					color_to_fcolor(hit_record->color),
					*data.power
					),
				compute_light_v2(hit_record, minirt)
				)
			);
	*data.power = multiply_fcolor(
			*data.power,
			color_to_fcolor(hit_record->color)
			);
}

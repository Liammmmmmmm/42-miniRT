/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:34:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/09 12:26:27 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

double	get_cos_theta(t_vec3 dir_in, t_vec3 normal)
{
	return (-vec3_dot(vec3_unit(dir_in), normal));
}

t_vec3	reflection_vec(const t_vec3 uv, const t_vec3 n)
{
	t_vec3	direction;

	direction = vec3_subtract(uv, vec3_multiply_scalar(n, 2 * vec3_dot(uv, n)));
	return (direction);
}

inline double	get_reflect_value(t_ray *ray_in, t_hit_record *hit_record)
{
	return (fresnel_schlick_f(
			get_cos_theta(ray_in->dir, hit_record->normal),
			powf((hit_record->mat->ior - 1)
				/ (hit_record->mat->ior + 1), 2)
		)
	);
}

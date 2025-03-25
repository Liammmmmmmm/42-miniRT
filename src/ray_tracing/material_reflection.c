/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:04:00 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/25 14:40:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "maths.h"

char	default_texture(t_hit_record hit_record, t_ray *scatted)
{
	t_vec3	direction;

	direction = vec3_add(hit_record.normal, random_vec3_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
		direction = hit_record.normal;
	scatted->dir = direction;
	scatted->orig = hit_record.point;
	return (1);
}

char	metal_texture(t_hit_record hit_record, t_ray ray_in, t_ray *scatted)
{
	t_vec3	direction;

	direction = vec3_subtract(ray_in.dir, vec3_multiply_scalar(hit_record.normal, 2 * vec3_dot(ray_in.dir, hit_record.normal)));
	direction = vec3_add(vec3_unit(direction), vec3_multiply_scalar(random_vec3_unit(), hit_record.mat->roughness_value)); // TODO un jour on devra mettre roughness texture et la on va plus en chier x)
	scatted->dir = direction;
	scatted->orig = hit_record.point;
	return (vec3_dot(scatted->dir, hit_record.normal) > 0);
}

char	calc_ray_reflection(t_hit_record hit_record, t_ray ray_in, t_ray *scatted)
{
	if (hit_record.mat)
	{
		if (hit_record.mat->metallic_value == 1)
		{
			return (metal_texture(hit_record, ray_in, scatted));
		}
	}
	return (default_texture(hit_record, scatted));
}
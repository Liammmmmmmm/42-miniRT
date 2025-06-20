/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_light.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:07:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 17:08:06 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "material.h"
#include "bvh.h"
#include <math.h>

char	check_dlight_hit_dir(t_minirt *minirt, t_vec3 origin, t_vec3 direction)
{
	t_ray				shadow_ray;
	t_hit_register_data	data;

	shadow_ray.orig = origin;
	shadow_ray.dir = vec3_unit(direction);
	data.is_light = 1;
	data.hit_record.mat = NULL;
	data.ray = &shadow_ray;
	if (hit_register_all(minirt, &data))
		return (1);
	return (0);
}

void	add_dlight(t_lcolor *light_color, t_hit_record *hit, \
	t_dlight *light, t_vec3 view_dir)
{
	const t_vec3	light_dir = vec3_negate(light->orientation);
	const double	n_dot_l = vec3_dot(hit->normal, light_dir);
	t_vec3			reflect_dir;
	double			spec;
	double			r_dot_v;

	if (n_dot_l <= 0.0)
		return ;
	light_color->r += light->color.r * light->brightness * n_dot_l;
	light_color->g += light->color.g * light->brightness * n_dot_l;
	light_color->b += light->color.b * light->brightness * n_dot_l;
	reflect_dir = reflection_vec(vec3_negate(light_dir), hit->normal);
	r_dot_v = vec3_dot(reflect_dir, view_dir);
	if (r_dot_v < 0.0)
		r_dot_v = 0.0;
	spec = pow(r_dot_v, SHINE);
	light_color->r += light->color.r * light->brightness * SPEC_STRENGTH * spec;
	light_color->g += light->color.g * light->brightness * SPEC_STRENGTH * spec;
	light_color->b += light->color.b * light->brightness * SPEC_STRENGTH * spec;
}

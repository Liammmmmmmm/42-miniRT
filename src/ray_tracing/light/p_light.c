/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_light.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:07:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 16:24:32 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "material.h"
#include "bvh.h"
#include <math.h>

void	compute_shadow_factor(t_minirt *minirt, t_vec3 origin,
	t_light *light)
{
	t_ray				shadow_ray;
	t_hit_register_data	data;
	size_t				i;
	int					hits_count;
	t_vec3				random_target;

	hits_count = 0;
	i = 0;
	shadow_ray.orig = origin;
	data.is_light = 1;
	data.ray = &shadow_ray;
	data.hit_record.mat = NULL;
	while (i < light->shadow_sample)
	{
		random_target = vec3_add(light->position,
				vec3_multiply_scalar(random_in_unit_sphere(), light->radius));
		shadow_ray.dir = vec3_unit(vec3_subtract(random_target, origin));
		if (hit_register_all(minirt, &data) == 0 || data.hit_record.t
			>= vec3_length(vec3_subtract(random_target, origin)))
			hits_count++;
		i++;
	}
	light->shadow_factor = ((double)hits_count / (double)light->shadow_sample);
}

char	check_plight_hit(t_minirt *minirt, t_vec3 origin, t_vec3 target)
{
	t_ray				shadow_ray;
	t_hit_register_data	data;
	double				distance_to_target;

	data.is_light = 1;
	data.ray = &shadow_ray;
	data.hit_record.mat = NULL;
	shadow_ray.orig = origin;
	shadow_ray.dir = vec3_unit(vec3_subtract(target, origin));
	distance_to_target = vec3_length(vec3_subtract(target, origin));
	if (hit_register_all(minirt, &data))
		if (data.hit_record.t < distance_to_target)
			return (1);
	return (0);
}

void	add_plight(t_lcolor *light_color, t_hit_record *hit, t_light *light, \
	t_vec3 view_dir)
{
	const t_vec3	light_dir = vec3_unit(vec3_subtract(light->position, \
		hit ->point));
	const double	n_dot_l = vec3_dot(hit->normal, light_dir);
	double			spec;
	double			r_dot_v;
	double			diffuse_intensity;

	if (n_dot_l <= 0.0)
		return ;
	diffuse_intensity = light->brightness * n_dot_l * light->shadow_factor;
	light_color->r += light->color.r * diffuse_intensity;
	light_color->g += light->color.g * diffuse_intensity;
	light_color->b += light->color.b * diffuse_intensity;
	r_dot_v = vec3_dot(reflection_vec(vec3_negate(light_dir),
				hit->normal), view_dir);
	if (r_dot_v < 0.0)
		r_dot_v = 0.0;
	spec = pow(r_dot_v, SHINE) * SPEC_STRENGTH * light->shadow_factor;
	light_color->r += light->color.r * light->brightness * spec;
	light_color->g += light->color.g * light->brightness * spec;
	light_color->b += light->color.b * light->brightness * spec;
}

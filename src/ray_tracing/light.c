/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:27:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 13:45:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "material.h"
#include "bvh.h"
#include <math.h>

char	check_hit(t_minirt *minirt, t_vec3 origin, t_vec3 target)
{
	t_ray			shadow_ray;
	t_hit_record	hit_record;
	double			distance_to_target;

	shadow_ray.orig = origin;
	shadow_ray.dir = vec3_unit(vec3_subtract(target, origin));
	distance_to_target = vec3_length(vec3_subtract(target, origin));
	if (hit_bvh(&minirt->scene.bvh, 0, &shadow_ray, &hit_record))
	{
		if (hit_record.t < distance_to_target)
			return (1);
	}
	return (0);
}

void	clamp_light_color(t_lcolor *color)
{
	color->r = ft_dmin(color->r, 255);
	color->g = ft_dmin(color->g, 255);
	color->b = ft_dmin(color->b, 255);
}

void	add_light(t_lcolor *light_color, t_hit_record *hit, t_light *light, \
	t_vec3 view_dir)
{
	const t_vec3	light_dir = vec3_unit(vec3_subtract(light->position, \
		hit->point));
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

t_lcolor	compute_ambient(t_minirt *minirt)
{
	t_lcolor	ambient;

	ambient.r = minirt->scene.amb_light.color.r * minirt->scene.amb_light.ratio;
	ambient.g = minirt->scene.amb_light.color.g * minirt->scene.amb_light.ratio;
	ambient.b = minirt->scene.amb_light.color.b * minirt->scene.amb_light.ratio;
	return (ambient);
}

t_color	compute_light(t_hit_record *hit_record, t_minirt *minirt)
{
	t_lcolor	light_color;
	t_vec3		view_dir;
	t_light		*light;
	int			i;

	light_color = compute_ambient(minirt);
	view_dir = vec3_unit(vec3_subtract(minirt->scene.camera.position, \
		hit_record->point));
	i = 0;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == LIGHT)
		{
			light = minirt->scene.elements[i].object;
			if (check_hit(minirt, hit_record->point, light->position) == 0)
				add_light(&light_color, hit_record, light, view_dir);
		}
		i++;
	}
	clamp_light_color(&light_color);
	return ((t_color){light_color.r, light_color.g, light_color.b});
}

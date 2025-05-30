/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:27:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/30 18:11:55 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "material.h"
#include "bvh.h"
#include <math.h>

char	check_light_hit(t_minirt *minirt, t_vec3 origin, t_vec3 target)
{
	t_ray				shadow_ray;
	t_hit_register_data	data;
	double				distance_to_target;

	data.is_light = 1;
	data.ray = &shadow_ray;
	data.hit_record.mat = NULL;
	data.interval.max = 1000; // add la render distance
	data.interval.min = 0.001; // add la render distance
	shadow_ray.orig = origin;
	shadow_ray.dir = vec3_unit(vec3_subtract(target, origin));
	distance_to_target = vec3_length(vec3_subtract(target, origin));
	if (hit_register_all(minirt, &data))
		if (data.hit_record.t < distance_to_target)
			return (1);
	return (0);
}

char	check_dlight_hit_dir(t_minirt *minirt, t_vec3 origin, t_vec3 direction)
{
	t_ray				shadow_ray;
	t_hit_register_data	data;

	shadow_ray.orig = origin;
	shadow_ray.dir = vec3_unit(direction);
	data.is_light = 1;
	data.hit_record.mat = NULL;
	data.ray = &shadow_ray;
	data.interval.max = 1000; // add la render distance
	data.interval.min = 0.001; // add la render distance
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

t_fcolor	compute_light_v2(t_hit_record *hit_record, t_minirt *minirt)
{
	t_lcolor	light_color;
	t_vec3		view_dir;
	t_light		*light;
	t_dlight	*dlight;
	int			i;

	light_color = (t_lcolor){0, 0, 0};
	view_dir = vec3_unit(vec3_subtract(minirt->scene.camera.position, \
		hit_record->point));
	i = 0;
	while (i < minirt->scene.obj_lst.light_nb)
	{
		if (minirt->scene.obj_lst.light_lst[i]->type == LIGHT)
		{
			light = (t_light *)minirt->scene.obj_lst.light_lst[i]->object;
			if (check_light_hit(minirt, hit_record->point, light->position) == 0)
				add_light(&light_color, hit_record, light, view_dir);
		}
		if (minirt->scene.obj_lst.light_lst[i]->type == DIRECTIONAL_LIGHT)
		{
			dlight = (t_dlight *)minirt->scene.obj_lst.light_lst[i]->object;
			if (check_dlight_hit_dir(minirt, hit_record->point, vec3_negate(dlight->orientation)) == 0)
				add_dlight(&light_color, hit_record, dlight, view_dir);
		}
		i++;
	}
	return ((t_fcolor){light_color.r / 255.0, light_color.g / 255.0, light_color.b / 255.0});
}

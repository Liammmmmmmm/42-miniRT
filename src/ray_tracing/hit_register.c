/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/14 16:18:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	get_hit_register_color(t_mat *mat, t_color color, t_hit_record *hit)
{
	if (mat)
	{
		if (mat->color_tex)
		{
			if (!mat->color_tex->img.pixel_data || !mat->color_tex->img.width || !mat->color_tex->img.height)
				return (get_solid_texture(hit->point, 2));
			else
			{
				return (mat->color_tex->img.pixel_data[mat->color_tex->img.width * (int)(hit->v * mat->color_tex->img.height) + (int)(hit->u * mat->color_tex->img.width)]);
			}
		}
		else
			return (mat->color_value);
	}
	return (color);
}

void	apply_normal_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->normal == NULL)
		return ;
	map = hit->mat->normal->img.pixel_data[hit->mat->normal->img.width * (int)(hit->v * hit->mat->normal->img.height) + (int)(hit->u * hit->mat->normal->img.width)];
	
	t_vec3 normal_map;
	normal_map.x = (map.r / 127.5f) - 1.0f;
	normal_map.y = (map.g / 127.5f) - 1.0f;
	normal_map.z = (map.b / 127.5f) - 1.0f;

	t_vec3 T;
	if (fabs(hit->normal.x) > 0.9f)
		T = (t_vec3){0, 1, 0};
	else
		T = (t_vec3){1, 0, 0};
	T = vec3_unit(vec3_cross(T, hit->normal));
	
	t_vec3 B = vec3_cross(hit->normal, T);

	
	hit->normal = vec3_add(
		vec3_add(
			vec3_multiply_scalar(T, normal_map.x),
			vec3_multiply_scalar(B, normal_map.y)),
		vec3_multiply_scalar(hit->normal, normal_map.z)
	);
}

char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record)
{
	t_hit_record	temp_hit_record;
	t_sphere		*sphere;
	t_plane			*plane;
	t_interval		interval;
	double			closest_t;
	int				hit_anything;
	int				i;

	i = 0;
	hit_anything = 0;
	interval.min = 0.001;
	interval.max = 1000;
	closest_t = INFINITY;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
		{
			sphere = minirt->scene.elements[i].object;
			if (hit_sphere(*sphere, &ray, interval, &temp_hit_record))
			{
				if (temp_hit_record.t < closest_t)
				{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					hit_record->mat = sphere->material;
					apply_normal_map(hit_record);
					hit_record->color = get_hit_register_color(sphere->material, sphere->color, hit_record);
				}
			}
		}
		if (minirt->scene.elements[i].type == PLANE)
		{
			plane = minirt->scene.elements[i].object;
			if (hit_plane(plane->position, plane->normal, &ray, interval, &temp_hit_record))
			{
				if (temp_hit_record.t < closest_t)
				{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					hit_record->mat = plane->material;
					hit_record->color = get_hit_register_color(plane->material, plane->color, hit_record);
				}
			}
		}
		i++;
	}
	return (hit_anything);
}

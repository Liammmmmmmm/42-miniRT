/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/30 17:39:05 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "bvh.h"
#include "hit_register.h"
#include <math.h>

char	hit_register_bvh(t_bvh *bvh, t_bvh_node *node, t_hit_register_data *data)
{
	t_hit_record	temp_hit_record;
	char			hit_anything;
	uint32_t		i;
	uint32_t		prim_index;
	double			closest_t;
	t_object		*obj;

	i = (uint32_t)-1;
	hit_anything = 0;
	data->interval.min = 0.001;
	data->interval.max = 1000;
	closest_t = 1000;
	temp_hit_record.part = DEFAULT;
	while (++i < node->prim_count)
	{
		prim_index = bvh->prim_indices[node->first_prim + i];
		obj = bvh->obj_list[prim_index];
		if (obj->type == SPHERE && hit_sphere(((t_sphere *)obj->object), data->ray, data->interval, &temp_hit_record))
		{			
			if (temp_hit_record.t >= closest_t)
				continue ;
			temp_hit_record.mat = ((t_sphere *)obj->object)->material;
		}
		else if (obj->type == CYLINDER && hit_cylinder(((t_cylinder *)obj->object), data->ray, data->interval, &temp_hit_record))
		{
			if (temp_hit_record.t >= closest_t)
				continue ;
			if (temp_hit_record.part == TOP_CAP && ((t_cylinder *)obj->object)->material_top)
				temp_hit_record.mat = ((t_cylinder *)obj->object)->material_top;
			else if (temp_hit_record.part == BOTTOM_CAP && ((t_cylinder *)obj->object)->material_bot)
				temp_hit_record.mat = ((t_cylinder *)obj->object)->material_bot;
			else
				temp_hit_record.mat = ((t_cylinder *)obj->object)->material;
		}
		else if (obj->type == CONE && hit_cone(((t_cone *)obj->object), data->ray, data->interval, &temp_hit_record))
		{
			if (temp_hit_record.t >= closest_t)
				continue ;
			if (temp_hit_record.part == TOP_CAP && ((t_cone *)obj->object)->material_top)
				temp_hit_record.mat = ((t_cone *)obj->object)->material_top;
			else
				temp_hit_record.mat = ((t_cone *)obj->object)->material;
		}
		else if (obj->type == HYPERBOLOID && hit_hyperboloid(((t_hyperboloid *)obj->object), data->ray, data->interval, &temp_hit_record))
		{
			if (temp_hit_record.t >= closest_t)
				continue ;
			temp_hit_record.mat = ((t_hyperboloid *)obj->object)->material;
		}
		else if (obj->type == TRIANGLE && hit_triangle(((t_triangle *)obj->object), data->ray, data->interval, &temp_hit_record))
		{
			if (temp_hit_record.t >= closest_t)
				continue ;
			temp_hit_record.mat = ((t_triangle *)obj->object)->material;
		}
		else
			continue ;
		hit_anything = 1;
		closest_t = temp_hit_record.t;
		data->hit_record = temp_hit_record;
	}
	if (!hit_anything)
		return (0);
	apply_all_map(data);
	data->hit_record.obj = obj;
	data->hit_record.color = get_hit_register_color(data->hit_record.mat, get_obj_color(obj), &data->hit_record, bvh);
	return (1);
}

char	hit_register_all(t_minirt *minirt, t_hit_register_data *data)
{
	char			hit;
	int				i;
	t_plane			*plane;
	t_object		**plane_lst;
	t_hit_record	temp_hit_record;

	hit = 0;
	temp_hit_record.part = DEFAULT;
	if (minirt->scene.bvh.valid == 1)
		hit = hit_bvh(&minirt->scene.bvh, 0, data);
	i = 0;
	plane_lst = minirt->scene.obj_lst.plane_lst;
	while (i < minirt->scene.obj_lst.plane_nb)
	{
		plane = (t_plane *)plane_lst[i]->object;
		if (hit_plane(plane, data->ray, (t_interval){0.001, 1000}, &temp_hit_record))
		{
			if (hit == 0 || temp_hit_record.t < data->hit_record.t)
			{
				hit = 1;
				data->hit_record = temp_hit_record;
				data->hit_record.mat = plane->material;
				data->hit_record.obj = plane_lst[i];
				apply_all_map(data);
				data->hit_record.color = get_hit_register_color(plane->material, plane->color, &data->hit_record, &minirt->scene.bvh);
			}
		}
		i++;
	}
	return (hit);
}

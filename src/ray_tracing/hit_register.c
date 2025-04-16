/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 13:50:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

char	hit_register(t_bvh *bvh, t_bvh_node *node, t_ray *ray, t_hit_record *hit_record)
{
	t_hit_record	temp_hit_record;
	// t_plane			*plane;
	t_interval		interval;
	char			hit_anything;
	uint32_t		i;
	uint32_t		prim_index;
	double			closest_t;
	t_object		*obj;

	i = 0;
	hit_anything = 0;
	interval.min = 0.001;
	interval.max = 1000;
	closest_t = 1000;
	while (i < node->prim_count)
	{
		prim_index = bvh->prim_indices[node->first_prim + i];
		obj = &bvh->obj_list[prim_index];
		if (obj->type == SPHERE && hit_sphere(((t_sphere *)obj->object), ray, interval, &temp_hit_record))
		{			
			if (temp_hit_record.t < closest_t)
			{
				hit_anything = 1;
				closest_t = temp_hit_record.t;
				*hit_record = temp_hit_record;
				if (((t_sphere *)bvh->obj_list[prim_index].object)->material)
				{
					hit_record->mat = ((t_sphere *)bvh->obj_list[prim_index].object)->material;
					hit_record->color =  ((t_sphere *)bvh->obj_list[prim_index].object)->material->color_value;
				}
				else
				{
					hit_record->mat = NULL;
					hit_record->color = ((t_sphere *)bvh->obj_list[prim_index].object)->color;
				}
			}
		}
		// if (minirt->scene.elements[i].type == PLANE)
		// {
		// 	plane = minirt->scene.elements[i].object;
		// 	if (hit_plane(plane->position, plane->normal, &ray, interval, &temp_hit_record))
		// 	{
		// 		if (temp_hit_record.t < closest_t)
		// 		{
		// 			hit_anything = 1;
		// 			closest_t = temp_hit_record.t;
		// 			*hit_record = temp_hit_record;
		// 			if (plane->material)
		// 			{
		// 				hit_record->mat = plane->material;
		// 				hit_record->color = plane->material->color_value;
		// 			}
		// 			else
		// 			{
		// 				hit_record->mat = NULL;
		// 				hit_record->color = plane->color;
		// 			}
		// 		}
		// 	}
		// }
		i++;
	}
	return (hit_anything);
}

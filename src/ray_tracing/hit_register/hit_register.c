/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/17 11:42:28 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "bvh.h"
#include "hit_register.h"
#include <math.h>

static void	init_hit_data(t_hit_register_data *data, double *closest_t)
{
	*closest_t = 1000;
	data->hit_record.part = DEFAULT;
}

static char	hit_loop(t_bvh *bvh, t_bvh_node *node,
	t_hit_register_data *data, double *closest_t)
{
	uint32_t	i;
	t_object	*obj;
	uint32_t	prim_index;
	char		hit;

	hit = 0;
	i = -1;
	while (++i < node->prim_count)
	{
		prim_index = bvh->prim_indices[node->first_prim + i];
		obj = bvh->obj_list[prim_index];
		if (handle_hit(obj, data, closest_t, &data->hit_record))
		{
			data->hit_record.obj = obj;
			hit = 1;
		}
	}
	return (hit);
}

char	hit_register_bvh(t_bvh *bvh, t_bvh_node *node,
	t_hit_register_data *data)
{
	double	closest_t;
	double	transparency;

	init_hit_data(data, &closest_t);
	if (!hit_loop(bvh, node, data, &closest_t))
		return (0);
	if (data->hit_record.mat && data->hit_record.mat->color_tex)
	{
		transparency = get_tex_transparency(data->hit_record.mat->color_tex,
				data->hit_record.u, data->hit_record.v);
		if (transparency == 1)
			return (0);
		else if (transparency != 0)
		{
			if (random_double() < transparency)
				return (0);
		}
	}
	apply_all_map(&data->hit_record, data->is_light);
	data->hit_record.color = get_hit_register_color(
			data->hit_record.mat,
			get_obj_color(data->hit_record.obj),
			&data->hit_record,
			bvh);
	return (1);
}

static void	hit_all_planes(t_minirt *minirt, t_hit_register_data *data,
	t_hit_record *tmp, char *hit)
{
	int			i;
	t_plane		*plane;
	t_object	**plane_lst;

	i = 0;
	plane_lst = minirt->scene.obj_lst.plane_lst;
	while (i < minirt->scene.obj_lst.plane_nb)
	{
		plane = (t_plane *)plane_lst[i]->object;
		if (hit_plane(plane, data->ray, tmp))
		{
			if (*hit == 0 || tmp->t < data->hit_record.t)
			{
				*hit = 1;
				data->hit_record = *tmp;
				data->hit_record.mat = plane->material;
				data->hit_record.obj = plane_lst[i];
				apply_all_map(&data->hit_record, data->is_light);
				data->hit_record.color = get_hit_register_color(
						plane->material, plane->color, &data->hit_record,
						&minirt->scene.bvh);
			}
		}
		i++;
	}
}

char	hit_register_all(t_minirt *minirt, t_hit_register_data *data)
{
	char			hit;
	t_hit_record	tmp;

	hit = 0;
	data->depth = 0;
	tmp.part = DEFAULT;
	if (minirt->scene.bvh.valid == 1)
		hit = hit_bvh(&minirt->scene.bvh, 0, data, &data->depth);
	if (hit == 0)
		data->depth = 0;
	hit_all_planes(minirt, data, &tmp, &hit);
	return (hit);
}

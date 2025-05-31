/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_make_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:19:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/31 12:32:18 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "minirt.h"

static void	update_obj_custom_material(t_custom_object *obj)
{
	uint32_t	k;

	k = 0;
	while (k < obj->triangle_count)
	{
		((t_triangle *)(obj->obj_list[k]).object)->material = obj->material;
		((t_triangle *)(obj->obj_list[k]).object)->color = obj->color;
		k++;
	}
}

static	void	add_obj_custom(t_object *obj_list, t_bvh *bvh, size_t *i,
	size_t *j)
{
	size_t		k;

	ft_update_obj_position(obj_list[*j].object);
	update_obj_custom_material(obj_list[*j].object);
	k = 0;
	while (k < ((t_custom_object *)obj_list[*j].object)->triangle_count)
	{
		bvh->prim_indices[*i] = *i;
		bvh->obj_list[(*i)++]
			= &((t_custom_object *)obj_list[*j].object)->obj_list[k];
		k++;
	}
}

void	bvh_make_lst(size_t obj_c, t_object *obj_list, t_bvh *bvh)
{
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	while (j < obj_c)
	{
		if (obj_list[j].type == SPHERE || obj_list[j].type == CYLINDER || \
			obj_list[j].type == CONE || obj_list[j].type == HYPERBOLOID)
		{
			bvh->prim_indices[i] = i;
			bvh->obj_list[i++] = &obj_list[j];
		}
		else if (obj_list[j].type == CUSTOM)
			add_obj_custom(obj_list, bvh, &i, &j);
		j++;
	}
}

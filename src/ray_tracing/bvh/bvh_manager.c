/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:26:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 14:15:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "structs.h"
#include "maths.h"
#include <math.h>

void	split_bvh_node(t_bvh *bvh, uint32_t index, uint32_t start, \
	uint32_t count)
{
	const t_aabb	bounds = bvh->bvh_nodes[index].node_bounds;
	const t_vec3	extent = vec3_subtract(bounds.max, bounds.min);
	int				axis;
	uint32_t		left;
	uint32_t		right;

	if (extent.x > extent.y && extent.x > extent.z)
		axis = 0;
	else if (extent.y > extent.z)
		axis = 1;
	else
		axis = 2;
	qsort_axis(bvh->prim_indices + start, \
		(t_interval){0, count - 1}, bvh, axis);
	left = build_bvh(bvh, start, (start + count / 2) - start);
	right = build_bvh(bvh, (start + count / 2), start + count - (start + \
		count / 2));
	bvh->bvh_nodes[index].left_child = left;
	bvh->bvh_nodes[index].right_child = right;
}

t_bvh_node	init_bvh_node(t_bvh *bvh, uint32_t start, uint32_t count)
{
	t_bvh_node	node;
	t_aabb		bounds;
	t_aabb		b;
	uint32_t	i;

	bounds = \
		compute_object_bounds(&bvh->obj_list[bvh->prim_indices[start]]);
	i = 0;
	while (i < count)
	{
		b = compute_object_bounds(&bvh->obj_list[bvh->prim_indices[start + i]]);
		bounds.min = vec3_fmin(bounds.min, b.min);
		bounds.max = vec3_fmax(bounds.max, b.max);
		i++;
	}
	node.node_bounds = bounds;
	node.first_prim = start;
	node.prim_count = count;
	node.is_leaf = (count == 1);
	node.left_child = 0;
	node.right_child = 0;
	return (node);
}

uint32_t	build_bvh(t_bvh *bvh, uint32_t start, uint32_t count)
{
	t_bvh_node	node;
	uint32_t	index;

	node = init_bvh_node(bvh, start, count);
	index = bvh->bvh_nodes_used++;
	bvh->bvh_nodes[index] = node;
	if (!node.is_leaf)
		split_bvh_node(bvh, index, start, count);
	return (index);
}

void	init_bvh(t_bvh *bvh, t_object *obj_list, uint32_t obj_c)
{
	uint32_t		i;
	uint32_t		j;
	const uint32_t	count = count_object(obj_list, obj_c);

	ft_bzero(bvh, sizeof(t_bvh));
	if (count == 0)
		return ;
	if (init_bvh_malloc(bvh, obj_c) == 1)
		return ;
	bvh->valid = 1;
	i = 0;
	j = 0;
	while (j < obj_c)
	{
		if (obj_list[j].type == SPHERE || obj_list[j].type == CYLINDER || \
			obj_list[j].type == CONE)
		{
			bvh->prim_indices[i] = i;
			bvh->obj_list[i++] = obj_list[j];
		}
		j++;
	}
	build_bvh(bvh, 0, count);
}

char	hit_bvh(t_bvh *bvh, uint32_t node_index, t_ray *ray, \
	t_hit_record *hit_rec)
{
	char			hit_anything;
	char			hit_left;
	char			hit_right;
	t_hit_record	hit_tmp;
	t_bvh_node		*node;

	node = &bvh->bvh_nodes[node_index];
	if (!intersect_aabb(*ray, node->node_bounds))
		return (0);
	hit_anything = 0;
	if (node->is_leaf)
		hit_anything = hit_register_bvh(bvh, node, ray, hit_rec);
	else
	{
		hit_left = hit_bvh(bvh, node->left_child, ray, hit_rec);
		hit_right = hit_bvh(bvh, node->right_child, ray, &hit_tmp);
		if (hit_right)
			if ((hit_right && !hit_left) || (hit_tmp.t < hit_rec->t))
				*hit_rec = hit_tmp;
		hit_anything = hit_left || hit_right;
	}
	return (hit_anything);
}

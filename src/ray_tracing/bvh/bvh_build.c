/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:16:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/02 17:20:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "minirt.h"
#include "maths.h"

t_bvh_node	init_bvh_node(t_bvh *bvh, uint32_t start, uint32_t count)
{
	t_bvh_node	node;
	t_aabb		bounds;
	t_aabb		b;
	uint32_t	i;

	bounds = \
		compute_object_bounds(bvh->obj_list[bvh->prim_indices[start]]);
	i = 0;
	while (i < count)
	{
		b = compute_object_bounds(bvh->obj_list[bvh->prim_indices[start + i]]);
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

char	choose_axis(t_vec3 extent)
{
	if (extent.x > extent.y && extent.x > extent.z)
		return (0);
	else if (extent.y > extent.z)
		return (1);
	else
		return (2);
}

char	split_bvh_node(t_bvh *bvh, uint32_t index, t_bvh_task task, \
	t_bvh_node node)
{
	const t_aabb	bounds = node.node_bounds;
	const t_vec3	extent = vec3_subtract(bounds.max, bounds.min);
	const uint32_t	mid = task.start + task.count / 2;
	int				axis;
	t_bvh_task		new_task;

	axis = choose_axis(extent);
	qsort_axis(bvh->prim_indices + task.start, (t_interval){0, task.count - 1},
		bvh, axis);
	if (mid - task.start > 0)
	{
		new_task = (t_bvh_task){task.start, mid - task.start, index, 1};
		if (vector_add(&bvh->task_stack, &new_task) == -1)
			return (1);
	}
	if (task.start + task.count - mid > 0)
	{
		new_task = (t_bvh_task){mid, task.start + task.count - mid, index, 0};
		if (vector_add(&bvh->task_stack, &new_task) == -1)
			return (1);
	}
	return (0);
}

char	run_bvh_build_task(t_bvh *bvh, uint32_t *index, t_bvh_node *node)
{
	t_bvh_task	task;

	task = *(t_bvh_task *)at_vector(&bvh->task_stack, \
		--bvh->task_stack.num_elements);
	*index = bvh->bvh_nodes_used++;
	*node = init_bvh_node(bvh, task.start, task.count);
	bvh->bvh_nodes[*index] = *node;
	if (task.is_left)
		bvh->bvh_nodes[task.parent].left_child = *index;
	else
		bvh->bvh_nodes[task.parent].right_child = *index;
	if (node->is_leaf)
	{
		bvh->actual++;
		print_progress_bar(bvh->actual, bvh->size);
	}
	else
		if (split_bvh_node(bvh, *index, task, *node) == 1)
			return (1);
	return (0);
}

uint32_t	build_bvh(t_bvh *bvh, uint32_t start, uint32_t count)
{
	t_bvh_task	task;
	uint32_t	index;
	t_bvh_node	node;

	if (vector_init(&bvh->task_stack, sizeof(t_bvh_task), 64) == -1)
		return (1);
	index = bvh->bvh_nodes_used++;
	node = init_bvh_node(bvh, start, count);
	bvh->bvh_nodes[index] = node;
	task = (t_bvh_task){start, count, UINT32_MAX, 0};
	if (!node.is_leaf)
		split_bvh_node(bvh, index, task, node);
	while (bvh->task_stack.num_elements > 0)
	{
		if (run_bvh_build_task(bvh, &index, &node) == 1)
			return (1);
	}
	return (0);
}

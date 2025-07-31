/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_task.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:21:32 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/31 18:30:22 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

static void	create_leaf_node(t_kd_tree *tree, int *current_node_idx,
	t_kd_build_task *task)
{
	t_kd_node	*node;

	node = &tree->nodes[(*current_node_idx)++];
	*(task->parent_link) = node;
	node->axis = -1;
	node->aabb_min = task->aabb_min;
	node->aabb_max = task->aabb_max;
	node->data.leaf.photon_start_idx = task->start;
	node->data.leaf.photon_count = task->end - task->start + 1;
}

static void	create_internal_node(t_kd_tree *tree, int *current_node_idx,
	t_kd_task_data *data)
{
	int			axis;
	int			median_index;
	t_kd_node	*node;
	float		split_pos;

	axis = data->task.depth % 3;
	median_index = data->task.start + (data->task.end - data->task.start) / 2;
	find_median_photon(tree->photons, (t_range){data->task.start, \
		data->task.end, median_index}, axis);
	node = &tree->nodes[(*current_node_idx)++];
	*(data->task.parent_link) = node;
	split_pos = get_pos_on_axis(&tree->photons[median_index], axis);
	node->axis = axis;
	node->aabb_min = data->task.aabb_min;
	node->aabb_max = data->task.aabb_max;
	node->data.internal.split_position = split_pos;
	data->stack[(*data->stack_top)].start = median_index + 1;
	data->stack[(*data->stack_top)].end = data->task.end;
	data->stack[(*data->stack_top)].depth = data->task.depth + 1;
	data->stack[(*data->stack_top)].parent_link = &node->data.internal.right;
	data->stack[(*data->stack_top)].aabb_min = data->task.aabb_min;
	data->stack[(*data->stack_top)].aabb_max = data->task.aabb_max;
	if (axis == 0)
		data->stack[(*data->stack_top)].aabb_min.x = split_pos;
	else if (axis == 1)
		data->stack[(*data->stack_top)].aabb_min.y = split_pos;
	else
		data->stack[(*data->stack_top)].aabb_min.z = split_pos;
	(*data->stack_top)++;
	data->stack[(*data->stack_top)].start = data->task.start;
	data->stack[(*data->stack_top)].end = median_index - 1;
	data->stack[(*data->stack_top)].depth = data->task.depth + 1;
	data->stack[(*data->stack_top)].parent_link = &node->data.internal.left;
	data->stack[(*data->stack_top)].aabb_min = data->task.aabb_min;
	data->stack[(*data->stack_top)].aabb_max = data->task.aabb_max;
	if (axis == 0)
		data->stack[(*data->stack_top)].aabb_max.x = split_pos;
	else if (axis == 1)
		data->stack[(*data->stack_top)].aabb_max.y = split_pos;
	else
		data->stack[(*data->stack_top)].aabb_max.z = split_pos;
	(*data->stack_top)++;
}

int	process_task(t_kd_tree *tree, int *current_node_idx, t_kd_task_data data)
{
	if (data.task.start > data.task.end)
	{
		*(data.task.parent_link) = NULL;
		return (0);
	}
	if ((data.task.end - data.task.start + 1) <= MAX_PHOTONS_PER_LEAF)
		create_leaf_node(tree, current_node_idx, &data.task);
	else
		create_internal_node(tree, current_node_idx, &data);
	print_progress_bar(*current_node_idx, tree->photon_count);
	return (1);
}
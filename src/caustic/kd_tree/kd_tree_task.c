/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_task.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:21:32 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 22:04:22 by madelvin         ###   ########.fr       */
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
	node->u_data.t_leaf.photon_start_idx = task->start;
	node->u_data.t_leaf.photon_count = task->end - task->start + 1;
}

static void	push_subtasks(t_kd_task_data *data,
	float split_pos, int axis, t_kd_node *node)
{
	const int	mi = data->task.start + (data->task.end - data->task.start) / 2;
	int			*top;

	top = data->stack_top;
	data->stack[*top] = (t_kd_build_task){mi + 1, data->task.end,
		data->task.depth + 1, &node->u_data.t_internal.right,
		data->task.aabb_min, data->task.aabb_max};
	if (axis == 0)
		data->stack[*top].aabb_min.x = split_pos;
	else if (axis == 1)
		data->stack[*top].aabb_min.y = split_pos;
	else
		data->stack[*top].aabb_min.z = split_pos;
	(*top)++;
	data->stack[*top] = (t_kd_build_task){data->task.start, mi - 1,
		data->task.depth + 1, &node->u_data.t_internal.left,
		data->task.aabb_min, data->task.aabb_max};
	if (axis == 0)
		data->stack[*top].aabb_max.x = split_pos;
	else if (axis == 1)
		data->stack[*top].aabb_max.y = split_pos;
	else
		data->stack[*top].aabb_max.z = split_pos;
	(*top)++;
}

static void	create_internal_node(t_kd_tree *tree, int *current_node_idx,
	t_kd_task_data *data)
{
	const int	axis = data->task.depth % 3;
	const int	mi = data->task.start
		+ (data->task.end - data->task.start) / 2;
	float		split_pos;
	t_kd_node	*node;

	find_median_photon(tree->photons, (t_range){
		data->task.start, data->task.end, mi}, axis);
	node = &tree->nodes[(*current_node_idx)++];
	*(data->task.parent_link) = node;
	split_pos = get_pos_on_axis(&tree->photons[mi], axis);
	node->axis = axis;
	node->aabb_min = data->task.aabb_min;
	node->aabb_max = data->task.aabb_max;
	node->u_data.t_internal.split_position = split_pos;
	push_subtasks(data, split_pos, axis, node);
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

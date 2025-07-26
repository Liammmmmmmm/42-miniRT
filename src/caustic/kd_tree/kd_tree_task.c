/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_task.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:21:32 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 19:30:13 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

static int	handle_empty_task(t_kd_build_task task)
{
	if (task.start > task.end)
	{
		*(task.parent) = NULL;
		return (1);
	}
	return (0);
}

static void	create_node_and_push_children(t_kd_tree *tree,
	int *current_node_idx, t_kd_task_data *data)
{
	int			axis;
	int			median_index;
	t_kd_node	*node;

	axis = data->task.depth % 3;
	median_index = data->task.start + (data->task.end - data->task.start) / 2;
	find_median_photon(tree->photons, (t_range){data->task.start, \
		data->task.end, median_index}, axis);
	node = &tree->nodes[(*current_node_idx)++];
	node->photon = &tree->photons[median_index];
	node->axis = axis;
	*(data->task.parent) = node;
	data->stack[(*data->stack_top)].start = median_index + 1;
	data->stack[(*data->stack_top)].end = data->task.end;
	data->stack[(*data->stack_top)].depth = data->task.depth + 1;
	data->stack[(*data->stack_top)].parent = &node->right;
	(*data->stack_top)++;
	data->stack[(*data->stack_top)].start = data->task.start;
	data->stack[(*data->stack_top)].end = median_index - 1;
	data->stack[(*data->stack_top)].depth = data->task.depth + 1;
	data->stack[(*data->stack_top)].parent = &node->left;
	(*data->stack_top)++;
}

int	process_task(t_kd_tree *tree, int *current_node_idx, t_kd_task_data data)
{
	if (handle_empty_task(data.task))
		return (0);
	create_node_and_push_children(tree, current_node_idx, &data);
	print_progress_bar(*current_node_idx, tree->photon_count);
	return (1);
}

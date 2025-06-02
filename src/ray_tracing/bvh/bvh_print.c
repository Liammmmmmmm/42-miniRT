/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:48:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/02 15:42:16 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "structs.h"
#include "bvh.h"

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

static void	print_leaf(t_bvh *bvh, t_bvh_node *node, int depth)
{
	uint32_t	i;
	uint32_t	prim_index;
	t_sphere	*sphere;

	i = 0;
	while (i < node->prim_count)
	{
		prim_index = bvh->prim_indices[node->first_prim + i];
		sphere = (t_sphere *)bvh->obj_list[prim_index]->object;
		print_indent(depth);
		printf("Sphere at (%.2f %.2f %.2f)\n",
			sphere->position.x, sphere->position.y, sphere->position.z);
		i++;
	}
}

static void	print_node_info(uint32_t index, t_bvh_node *node)
{
	ft_printf("Node %u | Leaf: %d | Prims: %u", index, node->is_leaf, \
		node->prim_count);
	ft_printf(" | Bounds Min: (%.2f %.2f %.2f) Max: (%.2f %.2f %.2f)\n",
		node->node_bounds.min.x, node->node_bounds.min.y,
		node->node_bounds.min.z, node->node_bounds.max.x,
		node->node_bounds.max.y, node->node_bounds.max.z);
}

void	print_bvh(t_bvh *bvh, uint32_t node_index, int depth)
{
	t_bvh_node	*node;

	node = &bvh->bvh_nodes[node_index];
	print_indent(depth);
	print_node_info(node_index, node);
	if (node->is_leaf)
		print_leaf(bvh, node, depth + 1);
	else
	{
		print_bvh(bvh, node->left_child, depth + 1);
		print_bvh(bvh, node->right_child, depth + 1);
	}
}

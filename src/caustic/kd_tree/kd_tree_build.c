/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:29:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 19:34:47 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

static int	init_stack(t_kd_build_task *stack, t_kd_node **root,
	int photon_count)
{
	stack[0].start = 0;
	stack[0].end = photon_count - 1;
	stack[0].depth = 0;
	stack[0].parent = root;
	return (1);
}

static t_kd_node	*build_kdtree_iterative(t_kd_tree *tree)
{
	t_kd_build_task	*stack;
	int				stack_top;
	int				current_node_idx;
	t_kd_node		*root;

	stack = malloc(sizeof(t_kd_build_task) * tree->photon_count);
	if (!stack)
		return (NULL);
	root = NULL;
	stack_top = init_stack(stack, &root, tree->photon_count);
	current_node_idx = 0;
	while (stack_top > 0)
	{
		stack_top--;
		process_task(tree, &current_node_idx, (t_kd_task_data){
			stack[stack_top], stack, &stack_top});
	}
	free(stack);
	printf("\n");
	return (root);
}

char	kd_tree_build(t_kd_tree *tree, t_vector *photons_vector)
{
	if (!tree || !photons_vector || photons_vector->num_elements == 0)
		return (print_error1("Invalid photon data."));
	tree->photons = malloc(photons_vector->num_elements * sizeof(t_photon));
	if (!tree->photons)
		return (print_error1("Failed to allocate photon array."));
	ft_memcpy(tree->photons, photons_vector->data, photons_vector->num_elements
		* sizeof(t_photon));
	tree->photon_count = photons_vector->num_elements;
	tree->nodes = malloc(tree->photon_count * sizeof(t_kd_node));
	if (!tree->nodes)
	{
		free(tree->photons);
		tree->photons = NULL;
		return (print_error1("Failed to allocate KD Tree nodes."));
	}
	tree->root = build_kdtree_iterative(tree);
	if (!tree->root)
	{
		free(tree->photons);
		free(tree->nodes);
		tree->photons = NULL;
		tree->nodes = NULL;
		return (print_error1("Iterative KD Tree build failed."));
	}
	return (0);
}

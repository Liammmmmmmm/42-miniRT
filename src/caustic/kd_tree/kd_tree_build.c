/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: delmath <delmath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:29:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/01 15:59:12 by delmath          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

static void	compute_photons_aabb(t_photon *photons, int count,
	t_vec3 *min, t_vec3 *max)
{
	int	i;

	i = 0;
	if (count == 0)
		return ;
	*min = photons[0].position;
	*max = photons[0].position;
	while (i < count)
	{
		min->x = fmin(min->x, photons[i].position.x);
		min->y = fmin(min->y, photons[i].position.y);
		min->z = fmin(min->z, photons[i].position.z);
		max->x = fmax(max->x, photons[i].position.x);
		max->y = fmax(max->y, photons[i].position.y);
		max->z = fmax(max->z, photons[i].position.z);
		i++;
	}
}

static int	init_stack(t_kd_build_task *stack, t_kd_tree *tree,
	t_kd_node **root)
{
	stack[0].start = 0;
	stack[0].end = tree->photon_count - 1;
	stack[0].depth = 0;
	stack[0].parent_link = root;
	compute_photons_aabb(tree->photons, tree->photon_count, &stack[0].aabb_min,
		&stack[0].aabb_max);
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
	stack_top = init_stack(stack, tree, &root);
	current_node_idx = 0;
	while (stack_top > 0)
	{
		stack_top--;
		process_task(tree, &current_node_idx, (t_kd_task_data){
			stack[stack_top], stack, &stack_top});
	}
	free(stack);
	printf("\n");
	tree->node_count = current_node_idx;
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

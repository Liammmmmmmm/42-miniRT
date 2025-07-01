/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:29:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 13:41:19 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

static t_kd_node	*build_recursive(t_kd_node *nodes, int *current_node_idx,
	t_photon* photons, int start, int end, int depth)
{
	int			axis;
	int			median_index;
	int			l;
	int			r;
	t_kd_node	*node;

	if (start > end)
		return (NULL);

	// Votre algorithme de recherche de la médiane (Quickselect) reste identique.
	// --- Début du Quickselect ---
	axis = depth % 3;
	median_index = start + (end - start) / 2;
	l = start;
	r = end;
	while (l < r) {
		float pivot_val = get_pos_on_axis(&photons[r], axis);
		int i = l - 1;
		int j = r;
		for (;;) {
			while (get_pos_on_axis(&photons[++i], axis) < pivot_val);
			while (j > l && get_pos_on_axis(&photons[--j], axis) > pivot_val);
			if (i >= j) break;
			swap_photons(&photons[i], &photons[j]);
		}
		swap_photons(&photons[i], &photons[r]);
		if (i == median_index) break;
		else if (i < median_index) l = i + 1;
		else r = i - 1;
	}
	// --- Fin du Quickselect ---

	node = &nodes[*current_node_idx];
	(*current_node_idx)++;
	node->photon = &photons[median_index];
	node->axis = axis;
	node->left = build_recursive(nodes, current_node_idx, photons, start, median_index - 1, depth + 1);
	node->right = build_recursive(nodes, current_node_idx, photons, median_index + 1, end, depth + 1);
	return (node);
}

char    kd_tree_build(t_kd_tree *tree, t_vector *photons_vector)
{
    int current_node_idx;

    if (!tree || !photons_vector || photons_vector->num_elements == 0)
        return (print_error1("Invalid photon data."));
    tree->photons = malloc(photons_vector->num_elements * sizeof(t_photon));
    if (!tree->photons)
        return (print_error1("Failed to allocate photon array."));
    tree->nodes = malloc(photons_vector->num_elements * sizeof(t_kd_node));
    if (!tree->nodes)
        return (print_error1("Failed to allocate KD Tree nodes."));
    ft_memcpy(tree->photons, photons_vector->data, photons_vector->num_elements
			* sizeof(t_photon));
    tree->photon_count = photons_vector->num_elements;
    current_node_idx = 0;
    tree->root = build_recursive(tree->nodes, &current_node_idx, tree->photons,
            0, tree->photon_count - 1, 0);
    if (!tree->root)
        return (print_error1("Recursive KD Tree build failed."));
    return (0);
}

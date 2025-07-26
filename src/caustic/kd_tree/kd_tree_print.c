/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:27:32 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 19:33:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"

void	print_recursive(t_kd_node *node, int depth)
{
	int		i;
	char	axis_char;
	t_vec3	pos;

	if (!node)
		return ;
	i = -1;
	print_recursive(node->right, depth + 1);
	while (++i < depth)
		printf("    ");
	if (node->axis == 0)
		axis_char = 'X';
	else if (node->axis == 1)
		axis_char = 'Y';
	else
		axis_char = 'Z';
	pos = node->photon->position;
	printf("[%c]-(% .2f, % .2f, % .2f)\n", axis_char, pos.x, pos.y, pos.z);
	print_recursive(node->left, depth + 1);
}

void	kd_tree_print(t_kd_tree *tree)
{
	if (!tree || !tree->root)
	{
		printf("Arbre K-D est vide ou non initialisé.\n");
		return ;
	}
	printf("\n--- Structure de l'Arbre K-D ---\n");
	print_recursive(tree->root, 0);
	printf("--- Fin de la Structure ---\n\n");
}

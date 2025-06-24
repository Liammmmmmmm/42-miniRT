/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:26:58 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/24 15:29:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"

void	kd_tree_destroy(t_kd_tree *tree)
{
	if (!tree)
		return ;
	free(tree->photons);
	free(tree->nodes);
	tree->photons = NULL;
	tree->nodes = NULL;
	tree->root = NULL;
	tree->photon_count = 0;
}

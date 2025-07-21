/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_find_near.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:22:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/21 14:41:58 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"
#include "bvh.h"

static double	distance_squared(t_vec3 a, t_vec3 b)
{
	return ((a.x - b.x) * (a.x - b.x)
		+ (a.y - b.y) * (a.y - b.y)
		+ (a.z - b.z) * (a.z - b.z));
}

static void	knn_update_best(t_knn_search *search)
{
	size_t	i;

	search->farthest_idx = 0;
	i = 1;
	while (i < search->count)
	{
		if (search->results[i].dist_sq
			> search->results[search->farthest_idx].dist_sq)
			search->farthest_idx = i;
		i++;
	}
}

static void	knn_set_best(t_kd_node *node, t_knn_search *search, double dist_sq)
{
	if (search->count < search->k)
	{
		search->results[search->count].photon = node->photon;
		search->results[search->count].dist_sq = dist_sq;
		search->count++;
		if (search->count == search->k)
			knn_update_best(search);
	}
	else if (dist_sq < search->results[search->farthest_idx].dist_sq)
	{
		search->results[search->farthest_idx].photon = node->photon;
		search->results[search->farthest_idx].dist_sq = dist_sq;
		knn_update_best(search);
	}
}

void	knn_find_recursive(t_kd_node *n, t_knn_search *search, t_vec3 point)
{
	t_kd_node	*next_branch;
	t_kd_node	*other_branch;
	double		dist_sq;
	double		dist_from_plane;

	if (n == NULL)
		return ;
	dist_sq = distance_squared(n->photon->position, point);
	knn_set_best(n, search, dist_sq);
	if (get_axis(point, n->axis) < get_axis(n->photon->position, n->axis))
	{
		next_branch = n->left;
		other_branch = n->right;
	}
	else
	{
		next_branch = n->right;
		other_branch = n->left;
	}
	knn_find_recursive(next_branch, search, point);
	dist_from_plane = get_axis(point, n->axis)
		- get_axis(n->photon->position, n->axis);
	if ((dist_from_plane * dist_from_plane)
		< search->results[search->farthest_idx].dist_sq)
		knn_find_recursive(other_branch, search, point);
}

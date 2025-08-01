/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_find_near.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: delmath <delmath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:22:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/01 15:09:53 by delmath          ###   ########.fr       */
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

static void	knn_set_best(t_photon *photon, t_knn_search *search, double dist_sq)
{
	if (search->count < search->k)
	{
		search->results[search->count].photon = photon;
		search->results[search->count].dist_sq = dist_sq;
		search->count++;
		if (search->count == search->k)
			knn_update_best(search);
	}
	else if (dist_sq < search->results[search->farthest_idx].dist_sq)
	{
		search->results[search->farthest_idx].photon = photon;
		search->results[search->farthest_idx].dist_sq = dist_sq;
		knn_update_best(search);
	}
}

static int	intersects_sphere_aabb(t_vec3 point, double radius_sq, t_vec3 aabb_min, t_vec3 aabb_max)
{
	double dist_sq = 0.0;
	double v;

	v = point.x;
	if (v < aabb_min.x)
		dist_sq += (aabb_min.x - v) * (aabb_min.x - v);
	else if (v > aabb_max.x)
		dist_sq += (v - aabb_max.x) * (v - aabb_max.x);
	v = point.y;
	if (v < aabb_min.y)
		dist_sq += (aabb_min.y - v) * (aabb_min.y - v);
	else if (v > aabb_max.y)
		dist_sq += (v - aabb_max.y) * (v - aabb_max.y);
	v = point.z;
	if (v < aabb_min.z)
		dist_sq += (aabb_min.z - v) * (aabb_min.z - v);
	else if (v > aabb_max.z)
		dist_sq += (v - aabb_max.z) * (v - aabb_max.z);
	return (dist_sq <= radius_sq);
}

void	knn_find_recursive(t_kd_node *n, t_knn_search *search, t_vec3 point, t_photon *photons)
{
	if (n == NULL)
		return ;
	if (search->count == search->k && !intersects_sphere_aabb(point, \
	search->results[search->farthest_idx].dist_sq, n->aabb_min, n->aabb_max))
		return;
	if (n->axis == -1)
	{
		for (int i = 0; i < n->data.leaf.photon_count; i++)
		{
			t_photon *photon = &photons[n->data.leaf.photon_start_idx + i];
			double dist_sq = distance_squared(photon->position, point);
			knn_set_best(photon, search, dist_sq);
		}
		return ;
	}
	t_kd_node	*next_branch;
	t_kd_node	*other_branch;
	double		dist_from_plane;

	dist_from_plane = get_axis(point, n->axis) - n->data.internal.split_position;
	if (dist_from_plane < 0)
	{
		next_branch = n->data.internal.left;
		other_branch = n->data.internal.right;
	}
	else
	{
		next_branch = n->data.internal.right;
		other_branch = n->data.internal.left;
	}
	knn_find_recursive(next_branch, search, point, photons);
	if (search->count < search->k || (dist_from_plane * dist_from_plane)
		< search->results[search->farthest_idx].dist_sq)
		knn_find_recursive(other_branch, search, point, photons);
}

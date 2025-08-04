/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_caustic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: delmath <delmath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:25:16 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/01 15:14:26 by delmath          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

static int	init_knn_search(t_knn_search *search, int k_nearest,
		t_knn_result **results_array)
{
	size_t	i;

	*results_array = malloc(sizeof(t_knn_result) * k_nearest);
	if (!*results_array)
		return (0);
	search->results = *results_array;
	search->k = k_nearest;
	search->count = 0;
	search->farthest_idx = 0;
	i = 0;
	while (i < (size_t)k_nearest)
	{
		search->results[i].dist_sq = DBL_MAX;
		i++;
	}
	return (1);
}

static t_fcolor	calculate_caustic_radiance(t_knn_search *search)
{
	t_fcolor	sum_photon_power;
	double		search_radius_sq;
	double		surface_area;
	size_t		i;

	search_radius_sq = search->results[search->farthest_idx].dist_sq;
	if (search_radius_sq > CAUSTIC_RADIUS)
		return ((t_fcolor){0.0, 0.0, 0.0});
	surface_area = M_PI * search_radius_sq;
	if (surface_area < 1e-9)
		return ((t_fcolor){0.0, 0.0, 0.0});
	sum_photon_power = (t_fcolor){0.0, 0.0, 0.0};
	i = 0;
	while (i < search->count)
	{
		sum_photon_power.r += search->results[i].photon->power.r;
		sum_photon_power.g += search->results[i].photon->power.g;
		sum_photon_power.b += search->results[i].photon->power.b;
		i++;
	}
	sum_photon_power.r = (sum_photon_power.r / surface_area) / NORM_FACTOR;
	sum_photon_power.g = (sum_photon_power.g / surface_area) / NORM_FACTOR;
	sum_photon_power.b = (sum_photon_power.b / surface_area) / NORM_FACTOR;
	return (sum_photon_power);
}

t_fcolor	estimate_caustics_physically_based(t_kd_tree *caustic_kdtree,
		t_vec3 point, int k_nearest)
{
	t_knn_search	search;
	t_knn_result	*results_array;
	t_fcolor		caustic_color;

	if (!caustic_kdtree || !caustic_kdtree->root || k_nearest <= 0)
		return ((t_fcolor){0.0, 0.0, 0.0});
	if (!init_knn_search(&search, k_nearest, &results_array))
		return ((t_fcolor){0.0, 0.0, 0.0});
	knn_find_recursive(caustic_kdtree->root, &search, point,
		caustic_kdtree->photons);
	if (search.count < (size_t)k_nearest)
	{
		free(results_array);
		return ((t_fcolor){0.0, 0.0, 0.0});
	}
	caustic_color = calculate_caustic_radiance(&search);
	free(results_array);
	return (caustic_color);
}

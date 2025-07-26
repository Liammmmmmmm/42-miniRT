/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_build_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:31:17 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 19:34:21 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

void	swap_photons(t_photon *a, t_photon *b)
{
	t_photon	t;

	t = *a;
	*a = *b;
	*b = t;
}

float	get_pos_on_axis(t_photon *p, int axis)
{
	if (axis == 0)
		return (p->position.x);
	if (axis == 1)
		return (p->position.y);
	return (p->position.z);
}

int	partition_photons(t_photon *p, int l, int r, int a)
{
	float	pivot;
	int		i;
	int		j;

	pivot = get_pos_on_axis(&p[r], a);
	i = l - 1;
	j = r;
	while (1)
	{
		while (get_pos_on_axis(&p[++i], a) < pivot)
			;
		while (j > l && get_pos_on_axis(&p[--j], a) > pivot)
			;
		if (i >= j)
			break ;
		swap_photons(&p[i], &p[j]);
	}
	swap_photons(&p[i], &p[r]);
	return (i);
}

void	find_median_photon(t_photon *p, t_range r, int a)
{
	int	i;

	while (r.start < r.end)
	{
		i = partition_photons(p, r.start, r.end, a);
		if (i == r.median)
			return ;
		else if (i < r.median)
			r.start = i + 1;
		else
			r.end = i - 1;
	}
}

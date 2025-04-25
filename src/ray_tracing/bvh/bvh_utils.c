/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:49:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 13:59:16 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>

uint32_t	count_object(t_object *obj_list, uint32_t obj_c)
{
	uint32_t	i;
	uint32_t	count;

	i = 0;
	count = 0;
	while (i < obj_c)
	{
		if (obj_list[i].type == SPHERE || obj_list[i].type == CYLINDER || \
			obj_list[i].type == CONE)
			count++;
		i++;
	}
	return (count);
}

t_interval	get_axis_interval(t_axis in)
{
	t_interval	res;

	if (in.dir != 0.0)
	{
		res.min = (in.min - in.orig) / in.dir;
		res.max = (in.max - in.orig) / in.dir;
	}
	else
	{
		res.min = -INFINITY;
		res.max = INFINITY;
	}
	return (res);
}

static inline void	update_interval(t_interval i, double *tmin, double *tmax)
{
	*tmin = fmax(*tmin, fmin(i.min, i.max));
	*tmax = fmin(*tmax, fmax(i.min, i.max));
}

inline char	intersect_aabb(t_ray ray, t_aabb aabb)
{
	double	tmin;
	double	tmax;

	tmin = -INFINITY;
	tmax = INFINITY;
	update_interval(get_axis_interval((t_axis){ray.orig.x, ray.dir.x, \
		aabb.min.x, aabb.max.x}), &tmin, &tmax);
	update_interval(get_axis_interval((t_axis){ray.orig.y, ray.dir.y, \
		aabb.min.y, aabb.max.y}), &tmin, &tmax);
	update_interval(get_axis_interval((t_axis){ray.orig.z, ray.dir.z, \
		aabb.min.z, aabb.max.z}), &tmin, &tmax);
	return (tmax >= tmin && tmin < 1000.0 && tmax > 0.0);
}

int	init_bvh_malloc(t_bvh *bvh, int count)
{
	bvh->obj_list = malloc(sizeof(t_object) * count);
	if (!bvh->obj_list)
	{
		ft_dprintf(2, "malloc failed in bvh_init\n");
		return (1);
	}
	bvh->bvh_nodes = malloc(sizeof(t_bvh_node) * count * 2);
	if (!bvh->bvh_nodes)
	{
		ft_dprintf(2, "malloc failed in bvh_init\n");
		free(bvh->obj_list);
		return (1);
	}
	bvh->prim_indices = malloc(sizeof(uint32_t) * count);
	if (!bvh->prim_indices)
	{
		ft_dprintf(2, "malloc failed in bvh_init\n");
		free(bvh->bvh_nodes);
		free(bvh->obj_list);
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:49:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/21 14:32:34 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include "minirt.h"
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
			obj_list[i].type == CONE || obj_list[i].type == HYPERBOLOID)
			count++;
		if (obj_list[i].type == CUSTOM)
			count += ((t_custom_object *)obj_list[i].object)->triangle_count;
		i++;
	}
	return (count);
}

inline char	intersect_aabb(const t_ray *r, const t_aabb *b)
{
	double	t1;
	double	tmax;
	double	tmin;

	t1 = fmin((b->min.x - r->orig.x) / r->dir.x, (b->max.x - r->orig.x)
			/ r->dir.x);
	tmin = fmax(t1, fmin((b->min.y - r->orig.y) / r->dir.y,
				(b->max.y - r->orig.y) / r->dir.y));
	t1 = fmin((b->min.z - r->orig.z) / r->dir.z, (b->max.z - r->orig.z)
			/ r->dir.z);
	tmin = fmax(tmin, t1);
	t1 = fmax((b->min.x - r->orig.x) / r->dir.x, (b->max.x - r->orig.x)
			/ r->dir.x);
	tmax = fmin(t1, fmax((b->min.y - r->orig.y) / r->dir.y,
				(b->max.y - r->orig.y) / r->dir.y));
	t1 = fmax((b->min.z - r->orig.z) / r->dir.z, (b->max.z - r->orig.z)
			/ r->dir.z);
	tmax = fmin(tmax, t1);
	return (tmax >= fmax(tmin, 0.0) && tmin < 1000.0);
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

inline t_vec3	get_obj_position(uint32_t i, t_bvh *bvh)
{
	t_object	*obj;

	obj = bvh->obj_list[i];
	if (obj->type == SPHERE)
		return (((t_sphere *)obj->object)->position);
	else if (obj->type == CYLINDER)
		return (((t_cylinder *)obj->object)->position);
	else if (obj->type == CONE)
		return (((t_cone *)obj->object)->position);
	else if (obj->type == HYPERBOLOID)
		return (((t_hyperboloid *)obj->object)->position);
	else if (obj->type == TRIANGLE)
		return (((t_triangle *)obj->object)->center);
	return ((t_vec3){0, 0, 0});
}

inline float	get_axis(t_vec3 pos, int axis)
{
	if (axis == 0)
		return (pos.x);
	else if (axis == 1)
		return (pos.y);
	return (pos.z);
}

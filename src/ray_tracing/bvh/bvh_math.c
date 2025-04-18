/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 14:22:19 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"

inline t_aabb	compute_sphere_bounds(t_sphere *s)
{
	t_vec3	rvec;

	rvec = (t_vec3){s->radius, s->radius, s->radius};
	return ((t_aabb){.min = vec3_subtract(s->position, rvec), \
					.max = vec3_add(s->position, rvec)});
}

int	compare_sphere_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis)
{
	const t_sphere	*sa = (t_sphere *)bvh->obj_list[a].object;
	const t_sphere	*sb = (t_sphere *)bvh->obj_list[b].object;
	float			va;
	float			vb;

	if (axis == 0)
	{
		va = sa->position.x;
		vb = sb->position.x;
	}
	else if (axis == 1)
	{
		va = sa->position.y;
		vb = sb->position.y;
	}
	else
	{
		va = sa->position.z;
		vb = sb->position.z;
	}
	if (va < vb)
		return (-1);
	return (va > vb);
}

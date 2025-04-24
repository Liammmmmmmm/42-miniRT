/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/24 13:54:24 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>

static inline	t_aabb compute_cylinder_bounds(t_cylinder *cyl)
{
	const t_vec3 axis_half = vec3_multiply_scalar(vec3_unit(cyl->orientation), \
		cyl->height * 0.5);
	const t_vec3 center1 = vec3_subtract(cyl->position, axis_half);
	const t_vec3 center2 = vec3_add(cyl->position, axis_half);
	const t_vec3 radius_vec = { cyl->diameter * 0.5, cyl->diameter * 0.5, \
		cyl->diameter * 0.5 };

	return (t_aabb){
		.min = vec3_subtract(vec3_min(center1, center2), radius_vec),
		.max = vec3_add(vec3_max(center1, center2), radius_vec)};
}


static inline t_aabb	compute_sphere_bounds(t_sphere *s)
{
	t_vec3	rvec;

	rvec = (t_vec3){s->radius, s->radius, s->radius};
	return ((t_aabb){.min = vec3_subtract(s->position, rvec), \
					.max = vec3_add(s->position, rvec)});
}

inline t_aabb	compute_object_bounds(t_object *obj)
{
	if (obj->type == SPHERE)
		return (compute_sphere_bounds(obj->object));
	return (compute_cylinder_bounds(obj->object));
}

int	compare_obj_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis)
{
	t_vec3	pos_a;
	t_vec3	pos_b;

	if (bvh->obj_list[a].type == SPHERE)
		pos_a = ((t_sphere *)bvh->obj_list[a].object)->position;
	else if (bvh->obj_list[a].type == CYLINDER)
		pos_a = ((t_cylinder *)bvh->obj_list[a].object)->position;

	if (bvh->obj_list[b].type == SPHERE)
		pos_b = ((t_sphere *)bvh->obj_list[b].object)->position;
	else if (bvh->obj_list[b].type == CYLINDER)
		pos_b = ((t_cylinder *)bvh->obj_list[b].object)->position;

	float va = (axis == 0) ? pos_a.x : (axis == 1) ? pos_a.y : pos_a.z;
	float vb = (axis == 0) ? pos_b.x : (axis == 1) ? pos_b.y : pos_b.z;

	if (va < vb)
		return (-1);
	return (va > vb);
}

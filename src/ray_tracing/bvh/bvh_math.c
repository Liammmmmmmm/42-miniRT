/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/23 11:20:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>

static inline t_aabb	compute_cylinder_bounds(t_cylinder *cyl)
{
	t_vec3	axis;
	t_vec3	e;
	double	r;
	double	len_sq;

	r = cyl->diameter * 0.5;
	axis = vec3_multiply_scalar(vec3_unit(cyl->orientation), cyl->height);
	len_sq = vec3_dot(axis, axis);
	e = vec3_multiply_scalar(
			vec3_sqrt(vec3_subtract((t_vec3){1, 1, 1},
				vec3_divide(vec3_multiply(axis, axis),
					(t_vec3){len_sq, len_sq, len_sq}))), r);
	return ((t_aabb){
		.min = vec3_min(vec3_subtract(cyl->position, e),
			vec3_subtract(vec3_add(cyl->position, axis), e)),
		.max = vec3_max(vec3_add(cyl->position, e),
			vec3_add(vec3_add(cyl->position, axis), e))
	});
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

	if (bvh->obj_list[a]->type == SPHERE)
		pos_a = ((t_sphere *)bvh->obj_list[a]->object)->position;
	else if (bvh->obj_list[a]->type == CYLINDER)
		pos_a = ((t_cylinder *)bvh->obj_list[a]->object)->position;

	if (bvh->obj_list[b]->type == SPHERE)
		pos_b = ((t_sphere *)bvh->obj_list[b]->object)->position;
	else if (bvh->obj_list[b]->type == CYLINDER)
		pos_b = ((t_cylinder *)bvh->obj_list[b]->object)->position;

	float va = (axis == 0) ? pos_a.x : (axis == 1) ? pos_a.y : pos_a.z;
	float vb = (axis == 0) ? pos_b.x : (axis == 1) ? pos_b.y : pos_b.z;

	if (va < vb)
		return (-1);
	return (va > vb);
}

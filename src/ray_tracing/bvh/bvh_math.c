/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 15:40:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>

static inline t_aabb compute_cylinder_bounds(t_cylinder *cyl)
{
	const t_vec3	axis = vec3_unit(cyl->orientation);
	const t_vec3	half = vec3_multiply_scalar(axis, cyl->height * 0.5);
	const double	r = cyl->diameter * 0.5;
	t_vec3			min;
	t_vec3			max;

	min = vec3_min(vec3_subtract(cyl->position, half), \
							vec3_add(cyl->position, half));
	max = vec3_max(vec3_subtract(cyl->position, half),  \
							vec3_add(cyl->position, half));
	min.x -= r * sqrt(1.0 - axis.x * axis.x);
	min.y -= r * sqrt(1.0 - axis.y * axis.y);
	min.z -= r * sqrt(1.0 - axis.z * axis.z);
	max.x += r * sqrt(1.0 - axis.x * axis.x);
	max.y += r * sqrt(1.0 - axis.y * axis.y);
	max.z += r * sqrt(1.0 - axis.z * axis.z);
	return ((t_aabb){min, max});
}

static inline t_aabb	compute_cone_bounds(t_cone *cone)
{
	const t_vec3 axis = vec3_unit(cone->orientation);
	const double r = cone->diameter * 0.5;
	t_vec3 min;
	t_vec3 max;

	min = vec3_min(cone->position, vec3_add(cone->position, \
		vec3_multiply_scalar(axis, cone->height)));
	max = vec3_max(cone->position, vec3_add(cone->position, \
		vec3_multiply_scalar(axis, cone->height)));
	min.x -= r * sqrt(1.0 - axis.x * axis.x);
	min.y -= r * sqrt(1.0 - axis.y * axis.y);
	min.z -= r * sqrt(1.0 - axis.z * axis.z);
	max.x += r * sqrt(1.0 - axis.x * axis.x);
	max.y += r * sqrt(1.0 - axis.y * axis.y);
	max.z += r * sqrt(1.0 - axis.z * axis.z);
	return ((t_aabb){min, max});
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
	if (obj->type == CONE)
		return (compute_cone_bounds(obj->object));
	return (compute_cylinder_bounds(obj->object));
}

int compare_obj_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis)
{
	t_vec3	pos_a;
	t_vec3	pos_b;
	float	va;
	float	vb;

	if (bvh->obj_list[a].type == SPHERE)
		pos_a = ((t_sphere *)bvh->obj_list[a].object)->position;
	else if (bvh->obj_list[a].type == CYLINDER)
		pos_a = ((t_cylinder *)bvh->obj_list[a].object)->position;
	else if (bvh->obj_list[a].type == CONE)
		pos_a = ((t_cone *)bvh->obj_list[a].object)->position;
	if (bvh->obj_list[b].type == SPHERE)
		pos_b = ((t_sphere *)bvh->obj_list[b].object)->position;
	else if (bvh->obj_list[b].type == CYLINDER)
		pos_b = ((t_cylinder *)bvh->obj_list[b].object)->position;
	else if (bvh->obj_list[b].type == CONE)
		pos_b = ((t_cone *)bvh->obj_list[b].object)->position;
	va = (axis == 0) ? pos_a.x : (axis == 1) ? pos_a.y : pos_a.z;
	vb = (axis == 0) ? pos_b.x : (axis == 1) ? pos_b.y : pos_b.z;
	if (va < vb)
		return -1;
	return (va > vb);
}
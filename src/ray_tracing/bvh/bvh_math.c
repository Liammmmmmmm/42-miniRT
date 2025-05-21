/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/20 13:42:59 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>

static inline void	set_vertex(t_aabb *aabb, t_vertex *verts)
{
	size_t	j;
	t_vec3	p;

	j = 0;
	while (j < 3)
	{
		p = verts[j].pos;
		if (p.x < aabb->min.x)
			aabb->min.x = p.x;
		if (p.y < aabb->min.y)
			aabb->min.y = p.y;
		if (p.z < aabb->min.z)
			aabb->min.z = p.z;
		if (p.x > aabb->max.x)
			aabb->max.x = p.x;
		if (p.y > aabb->max.y)
			aabb->max.y = p.y;
		if (p.z > aabb->max.z)
			aabb->max.z = p.z;
	j++;
	}
}

static inline t_aabb	compute_custom_obj(t_custom_object *obj)
{
	t_aabb		aabb;
	t_vertex	verts[3];
	size_t		i;

	i = 0;
	if (!obj || obj->triangle_count == 0 || !obj->triangles)
		return ((t_aabb){.min = {0, 0, 0}, .max = {0, 0, 0}});
	aabb.min = obj->triangles[0].v0.pos;
	aabb.max = obj->triangles[0].v0.pos;
	while (i < obj->triangle_count)
	{
		verts[0] = obj->triangles[i].v0;
		verts[1] = obj->triangles[i].v1;
		verts[2] = obj->triangles[i].v2;
		set_vertex(&aabb, verts);
		i++;
	}
	return (aabb);
}

static inline t_aabb	compute_triangle_bounds(t_triangle *t)
{
	return ((t_aabb){vec3_min(t->v0.pos, vec3_min(t->v1.pos, t->v2.pos)), \
	vec3_max(t->v0.pos, vec3_max(t->v1.pos, t->v2.pos))});
}

static inline t_aabb	compute_hyperboloid_bounds(t_hyperboloid *hyp)
{
	const t_vec3	axis = vec3_unit(hyp->orientation);
	const t_vec3	half_height_vec = vec3_multiply_scalar(axis, \
		hyp->height * 0.5);
	const double	r_max = fmax(hyp->a, hyp->b) * (sqrt(1.0 + \
		((hyp->height * 0.5) * (hyp->height * 0.5)) / (hyp->c * hyp->c)));
	const t_vec3	r_vec = {r_max * sqrt(1.0 - axis.x * axis.x), r_max * \
		sqrt(1.0 - axis.y * axis.y), r_max * sqrt(1.0 - axis.z * axis.z)};
	t_aabb			aabb;

	aabb.min = vec3_subtract(vec3_min(vec3_subtract(hyp->position, \
	half_height_vec), vec3_add(hyp->position, half_height_vec)), r_vec);
	aabb.max = vec3_add(vec3_max(vec3_subtract(hyp->position, \
	half_height_vec), vec3_add(hyp->position, half_height_vec)), r_vec);
	return (aabb);
}

static inline t_aabb	compute_cylinder_bounds(t_cylinder *cyl)
{
	const t_vec3	axis = vec3_unit(cyl->orientation);
	const t_vec3	half = vec3_multiply_scalar(axis, cyl->height * 0.5);
	const double	r = cyl->diameter * 0.5;
	t_vec3			min;
	t_vec3			max;

	min = vec3_min(vec3_subtract(cyl->position, half),
			vec3_add(cyl->position, half));
	max = vec3_max(vec3_subtract(cyl->position, half),
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
	const t_vec3	axis = vec3_unit(cone->orientation);
	const double	r = cone->diameter * 0.5;
	t_vec3			min;
	t_vec3			max;

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
	if (obj->type == HYPERBOLOID)
		return (compute_hyperboloid_bounds(obj->object));
	if (obj->type == CUSTOM)
		return (compute_custom_obj(obj->object));
	if (obj->type == TRIANGLE)
		return (compute_triangle_bounds(obj->object));
	return (compute_cylinder_bounds(obj->object));
}

int	compare_obj_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis)
{
	t_vec3	pos_a;
	t_vec3	pos_b;
	float	va;
	float	vb;

	pos_a = get_obj_position(a, bvh);
	pos_b = get_obj_position(b, bvh);
	va = get_axis_value(pos_a, axis);
	vb = get_axis_value(pos_b, axis);
	if (va < vb)
		return (-1);
	else if (va > vb)
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/21 14:32:34 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include "obj_parsing.h"
#include <math.h>

inline t_aabb	compute_object_bounds(t_object *obj)
{
	if (obj->type == SPHERE)
		return (compute_sphere_bounds(obj->object));
	if (obj->type == CONE)
		return (compute_cone_bounds(obj->object));
	if (obj->type == HYPERBOLOID)
		return (compute_hyperboloid_bounds(obj->object));
	if (obj->type == CUSTOM)
		return (compute_custom_obj(obj->object, 0));
	if (obj->type == TRIANGLE)
		return (compute_triangle_bounds(obj->object));
	if (obj->type == CYLINDER)
		return (compute_cylinder_bounds(obj->object));
	return ((t_aabb){(t_vec3){0, 0, 0}, (t_vec3){0, 0, 0}});
}

int	compare_obj_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis)
{
	t_vec3	pos_a;
	t_vec3	pos_b;
	float	va;
	float	vb;

	pos_a = get_obj_position(a, bvh);
	pos_b = get_obj_position(b, bvh);
	va = get_axis(pos_a, axis);
	vb = get_axis(pos_b, axis);
	if (va < vb)
		return (-1);
	else if (va > vb)
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_obj_bounds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:09:39 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/31 14:23:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "bvh.h"
#include "maths.h"

inline t_aabb	compute_triangle_bounds(t_triangle *t)
{
	return ((t_aabb){vec3_min(t->v0.pos, vec3_min(t->v1.pos, t->v2.pos)), \
	vec3_max(t->v0.pos, vec3_max(t->v1.pos, t->v2.pos))});
}

inline t_aabb	compute_hyperboloid_bounds(t_hyperboloid *hyp)
{
	const t_vec3	axis = vec3_unit(hyp->orientation);
	const t_vec3	half_height_vec = vec3_multiply_scalar(axis, \
		hyp->height * 0.5);
	const double	r_max = fmax(hyp->a, hyp->b) * sqrt(hyp->shape \
			+ ((hyp->height * 0.5) * (hyp->height * 0.5)) / (hyp->c * hyp->c));
	const t_vec3	r_vec = {r_max * sqrt(1.0 - axis.x * axis.x), r_max * \
		sqrt(1.0 - axis.y * axis.y), r_max * sqrt(1.0 - axis.z * axis.z)};
	t_aabb			aabb;

	aabb.min = vec3_subtract(vec3_min(vec3_subtract(hyp->position, \
	half_height_vec), vec3_add(hyp->position, half_height_vec)), r_vec);
	aabb.max = vec3_add(vec3_max(vec3_subtract(hyp->position, \
	half_height_vec), vec3_add(hyp->position, half_height_vec)), r_vec);
	return (aabb);
}

inline t_aabb	compute_cylinder_bounds(t_cylinder *cyl)
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

inline t_aabb	compute_cone_bounds(t_cone *cone)
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

inline t_aabb	compute_sphere_bounds(t_sphere *s)
{
	t_vec3	rvec;

	rvec = (t_vec3){s->radius, s->radius, s->radius};
	return ((t_aabb){.min = vec3_subtract(s->position, rvec), \
					.max = vec3_add(s->position, rvec)});
}

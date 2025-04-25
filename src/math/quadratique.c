/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratique.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:01:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 16:30:01 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

inline char	solve_quadratic(t_quadratic *q)
{
	double	sqrt_disc;

	q->disc = q->b * q->b - 4.0 * q->a * q->c;
	if (q->disc < 0.0)
		return (0);
	sqrt_disc = sqrt(q->disc);
	q->t0 = (-q->b - sqrt_disc) / (2.0 * q->a);
	q->t1 = (-q->b + sqrt_disc) / (2.0 * q->a);
	return (1);
}

inline char	init_cylinder_quadratic(t_quadratic *q, t_cylinder *cyl, t_ray *r)
{
	t_vec3	axis;
	t_vec3	oc;
	double	dd;
	double	oo;
	double	radius;

	axis = vec3_unit(cyl->orientation);
	oc = vec3_subtract(r->orig, cyl->position);
	dd = vec3_dot(r->dir, axis);
	oo = vec3_dot(oc, axis);
	radius = cyl->diameter * 0.5;

	q->a = vec3_dot(r->dir, r->dir) - dd * dd;
	q->b = 2.0 * (vec3_dot(r->dir, oc) - dd * oo);
	q->c = vec3_dot(oc, oc) - oo * oo - radius * radius;

	return (solve_quadratic(q));
}

inline char	init_cone_quadratic(t_quadratic *q, t_cone *cone, t_ray *r)
{
	t_vec3  axis;
	t_vec3  oc;
	double  dd;
	double  oo;
	double  radius_over_height;
	double  k;

	axis = vec3_unit(cone->orientation);
	oc = vec3_subtract(r->orig, cone->position);
	dd = vec3_dot(r->dir, axis);
	oo = vec3_dot(oc, axis);
	
	radius_over_height = (cone->diameter * 0.5) / cone->height;
	k = 1.0 + radius_over_height * radius_over_height;

	q->a = vec3_dot(r->dir, r->dir) - k * dd * dd;
	q->b = 2.0 * (vec3_dot(r->dir, oc) - k * dd * oo);
	q->c = vec3_dot(oc, oc) - k * oo * oo;

	return (solve_quadratic(q));
}
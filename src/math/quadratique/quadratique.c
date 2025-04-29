/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratique.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:01:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 17:48:06 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

inline char	solve_quadratic(t_quadratic *q)
{
	double	sqrt_disc;
	double	denom;

	q->disc = q->b * q->b - 4.0 * q->a * q->c;
	if (q->disc < 0.0)
		return (0);
	denom = 2.0 * q->a;
	sqrt_disc = sqrt(q->disc);
	q->t0 = (-q->b - sqrt_disc) / denom;
	q->t1 = (-q->b + sqrt_disc) / denom;
	return (1);
}

inline char	init_sphere_quadratic(t_quadratic *q, t_sphere *s, t_ray *r)
{
	const t_vec3	oc = vec3_subtract(r->orig, s->position);

	q->a = vec3_dot(r->dir, r->dir);
	q->b = 2.0 * vec3_dot(r->dir, oc);
	q->c = vec3_dot(oc, oc) - s->sqrt_radius;
	return (solve_quadratic(q));
}

inline char	init_cylinder_quadratic(t_quadratic *q, t_cylinder *cyl, t_ray *r)
{
	const t_vec3	axis = vec3_unit(cyl->orientation);
	const t_vec3	oc = vec3_subtract(r->orig, cyl->position);
	const double	radius = cyl->diameter * 0.5;

	q->dd = vec3_dot(r->dir, axis);
	q->oo = vec3_dot(oc, axis);
	q->a = vec3_dot(r->dir, r->dir) - q->dd * q->dd;
	q->b = 2.0 * (vec3_dot(r->dir, oc) - q->dd * q->oo);
	q->c = vec3_dot(oc, oc) - q->oo * q->oo - radius * radius;
	return (solve_quadratic(q));
}

inline char	init_cone_quadratic(t_quadratic *q, t_cone *cone, t_ray *r)
{
	const t_vec3	axis = vec3_unit(cone->orientation);
	const t_vec3	oc = vec3_subtract(r->orig, cone->position);
	const double	k = pow((cone->diameter * 0.5) / cone->height, 2);

	q->dd = vec3_dot(r->dir, axis);
	q->oo = vec3_dot(oc, axis);
	q->a = vec3_dot(r->dir, r->dir) - (1.0 + k) * q->dd * q->dd;
	q->b = 2.0 * (vec3_dot(r->dir, oc) - (1.0 + k) * q->dd * q->oo);
	q->c = vec3_dot(oc, oc) - (1.0 + k) * q->oo * q->oo;
	return (solve_quadratic(q));
}

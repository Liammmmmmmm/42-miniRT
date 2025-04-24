/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:54:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/24 14:07:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

static inline char	valid_t(t_quadratic *q, t_interval i)
{
	q->t_hit = q->t0;
	if (q->t_hit < i.min || q->t_hit > i.max)
	{
		q->t_hit = q->t1;
		if (q->t_hit < i.min || q->t_hit > i.max)
			return (0);
	}
	return (1);
}

static inline char	handle_cylinder_hit(t_cylinder *cyl, t_ray *r,
t_hit_record *rec, t_quadratic *q)
{
	const t_vec3	axis = vec3_unit(cyl->orientation);
	const t_vec3	p = ray_at(*r, q->t_hit);
	const t_vec3	base = vec3_subtract(cyl->position,
			vec3_multiply_scalar(axis, cyl->height * 0.5));
	const double	proj = vec3_dot(vec3_subtract(p, base), axis);
	t_vec3			outward;

	if (proj < 0.0 || proj > cyl->height)
		return (0);
	outward = vec3_subtract(vec3_subtract(p, base),
			vec3_multiply_scalar(axis, proj));
	outward = vec3_unit(outward);
	rec->t = q->t_hit;
	rec->point = p;
	rec->normal = set_normal_face(r, &outward, rec);
	return (1);
}

static inline char	cylinder_cap_bottom(t_ray *r, t_cylinder *cyl,
	t_interval interval, t_hit_record *rec)
{
	const t_vec3	plane_origin = vec3_subtract(cyl->position, \
		vec3_multiply_scalar(vec3_unit(cyl->orientation), cyl->height * 0.5));
	const double	d = vec3_dot(r->dir, cyl->orientation);
	double			t;
	t_vec3			hit_point;

	if (fabs(d) < 1e-6)
		return (0);
	t = -vec3_dot(vec3_subtract(r->orig, plane_origin), cyl->orientation) / d;
	if (t < interval.min || t > interval.max)
		return (0);
	hit_point = ray_at(*r, t);
	if (vec3_length(vec3_subtract(hit_point, plane_origin)) > \
			cyl->diameter * 0.5)
		return (0);
	rec->t = t;
	rec->point = hit_point;
	rec->normal = vec3_negate(cyl->orientation);
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	return (1);
}

static inline char	cylinder_cap_top(t_ray *r, t_cylinder *cyl, \
	t_interval interval, t_hit_record *rec)
{
	const t_vec3	plane_origin = vec3_add(cyl->position,
			vec3_multiply_scalar(vec3_unit(cyl->orientation),
				cyl->height * 0.5));
	const double	denom = vec3_dot(r->dir, cyl->orientation);
	t_vec3			hit_point;
	double			t;

	if (fabs(denom) < 1e-6)
		return (0);
	t = -vec3_dot(vec3_subtract(r->orig, plane_origin),
			cyl->orientation) / denom;
	if (t < interval.min || t > interval.max)
		return (0);
	hit_point = ray_at(*r, t);
	if (vec3_length(vec3_subtract(hit_point, plane_origin)) > \
			cyl->diameter * 0.5)
		return (0);
	rec->t = t;
	rec->point = hit_point;
	rec->normal = cyl->orientation;
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	return (1);
}

char	hit_cylinder(t_cylinder *cyl, t_ray *r, t_interval interval,
	t_hit_record *rec)
{
	t_quadratic	q;
	int			hit_cap_bottom;
	int			hit_cap_top;

	if (cyl->orientation.x == 0 && cyl->orientation.y == 0 && \
			cyl->orientation.z == 0)
		cyl->orientation.y = 1;
	if (!init_cylinder_quadratic(&q, cyl, r) || !valid_t(&q, interval))
		return (0);
	if (handle_cylinder_hit(cyl, r, rec, &q))
		return (1);
	hit_cap_bottom = cylinder_cap_bottom(r, cyl, interval, rec);
	hit_cap_top = cylinder_cap_top(r, cyl, interval, rec);
	if (!(hit_cap_bottom || hit_cap_top))
		return (0);
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->normal = set_normal_face(r, &rec->normal, rec);
	return (1);
}
/*


// if (hit_cap_bottom && hit_cap_top)
// {
//  dist_bottom = vec3_length(vec3_subtract(rec->point, r->orig));
//  if (dist_bottom < rec->t)
//  {
//      rec->t = dist_bottom;
//      rec->normal = cyl->orientation;
//  }
//  else
//      rec->normal = vec3_negate(cyl->orientation);
// }
// else
// {
//  if (hit_cap_bottom)
//      rec->normal = cyl->orientation;
//  else
//      rec->normal = vec3_negate(cyl->orientation);
// }
*/

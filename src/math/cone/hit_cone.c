/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:49:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 17:29:06 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

static inline char	handle_cone_hit(t_cone *cone, t_ray *r, t_hit_record *rec, \
	t_quadratic *q)
{
	const t_vec3	axis = vec3_unit(cone->orientation);
	const t_vec3	p = ray_at(*r, q->t_hit);
	const t_vec3	apex = cone->position;
	const double	proj = vec3_dot(vec3_subtract(p, apex), axis);
	t_vec3			normal;

	if (proj < 0.0 || proj > cone->height)
		return (0);
	normal = vec3_subtract(vec3_subtract(p, apex), vec3_multiply_scalar(axis, \
		(1.0 + ((cone->diameter * 0.5 / cone->height) * (cone->diameter * 0.5 \
			/ cone->height))) * proj));
	normal = vec3_unit(normal);
	rec->t = q->t_hit;
	rec->point = p;
	rec->normal = set_normal_face(r, &normal, rec);
	return (1);
}

static inline char	cone_cap(t_cone *cone, t_ray *r, t_interval interval, \
	t_hit_record *rec)
{
	const t_vec3	cap_center = vec3_add(cone->position, \
							vec3_multiply_scalar(vec3_unit(cone->orientation), \
							cone->height));
	const t_vec3	normal = vec3_unit(cone->orientation);
	const double	denom = vec3_dot(r->dir, normal);
	double			t;
	t_vec3			hit_point;

	if (fabs(denom) < 1e-6)
		return (0);
	t = vec3_dot(vec3_subtract(cap_center, r->orig), normal) / denom;
	if (t < interval.min || t > interval.max)
		return (0);
	hit_point = ray_at(*r, t);
	if (vec3_length(vec3_subtract(hit_point, cap_center)) > \
			(cone->diameter * 0.5))
		return (0);
	rec->t = t;
	rec->point = hit_point;
	rec->normal = normal;
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	return (1);
}

char	hit_cone(t_cone *cone, t_ray *r, t_interval interval, t_hit_record *rec)
{
	t_quadratic	q;
	int			hit_cap;

	if (cone->orientation.x == 0 && cone->orientation.y == 0 && \
		cone->orientation.z == 0)
		cone->orientation.y = 1;
	if (!init_cone_quadratic(&q, cone, r) || !valid_t(&q, interval))
		return (0);
	if (handle_cone_hit(cone, r, rec, &q))
		return (1);
	hit_cap = cone_cap(cone, r, interval, rec);
	if (!hit_cap)
		return (0);
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->normal = set_normal_face(r, &rec->normal, rec);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_hyperboloid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:50:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 12:47:19 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static inline void	get_hyperboloid_uv(t_hit_record *rec, t_hyperboloid *hyper)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, hyper->position);

	if (fabs(hyper->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, hyper->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, hyper->orientation));
	rec->u = clamp_double((atan2(vec3_dot(rel, vec3_cross(hyper->orientation, \
		right)), vec3_dot(rel, right)) + PI_D) / (2.0 * PI_D));
	rec->v = clamp_double((vec3_dot(vec3_subtract(rec->point, \
		hyper->position), hyper->orientation) / (hyper->height * 0.5)) * 0.5 + \
		0.5);
	if (hyper->material && hyper->material->scale != 1)
	{
		rec->u *= hyper->material->scale;
		rec->v *= hyper->material->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
}

static t_vec3	compute_normal(t_hyperboloid *hyp, t_vec3 to_p, t_vec3 axis)
{
	t_vec3	tmp;
	t_vec3	u;
	t_vec3	v;

	if (fabs(axis.y) < 0.99)
		tmp = (t_vec3){0, 1, 0};
	else
		tmp = (t_vec3){1, 0, 0};
	u = vec3_unit(vec3_cross(tmp, axis));
	v = vec3_cross(axis, u);

	return (vec3_unit(vec3_add(vec3_add(
			vec3_multiply_scalar(u, 2.0 * vec3_dot(to_p, u) / (hyp->a * hyp->a)),
			vec3_multiply_scalar(v, 2.0 * vec3_dot(to_p, v) / (hyp->b * hyp->b))),
			vec3_multiply_scalar(axis, -2.0 * vec3_dot(to_p, axis) / (hyp->c * hyp->c))
		)));
}

char	handle_hyperboloid_hit(t_hyperboloid *hyp, t_ray *r,
	t_hit_record *rec, t_quadratic *q)
{
	const t_vec3	axis = vec3_unit(hyp->orientation);
	const t_vec3	p = ray_at(*r, q->t_hit);
	const t_vec3	to_p = vec3_subtract(p, hyp->position);
	t_vec3			normal;

	if (!(fabs(vec3_dot(to_p, axis)) <= hyp->height * 0.5))
		return (0);
	normal = compute_normal(hyp, to_p, axis);
	rec->t = q->t_hit;
	rec->point = p;
	rec->normal = set_normal_face(r, &normal, rec);
	rec->front_face = vec3_dot(r->dir, normal) < 0;
	get_hyperboloid_uv(rec, hyp);
	return (1);
}

char	hit_hyperboloid(t_hyperboloid *hyp, t_ray *r, t_interval interval,
	t_hit_record *rec)
{
	t_quadratic	q;
	char		hit_anything;

	if (vec3_length_squared(hyp->orientation) < 1e-6)
		hyp->orientation = (t_vec3){0, 0, 1};
	if (!init_hyperboloid_quadratic(&q, hyp, r))
		return (0);
	hit_anything = 0;
	if (q.t0 > interval.min && q.t0 < interval.max)
	{
		q.t_hit = q.t0;
		if (handle_hyperboloid_hit(hyp, r, rec, &q))
		{
			hit_anything = 1;
			interval.max = q.t0;
		}
	}
	if (q.t1 > interval.min && q.t1 < interval.max)
	{
		q.t_hit = q.t1;
		if (handle_hyperboloid_hit(hyp, r, rec, &q))
			hit_anything = 1;
	}
	return (hit_anything);
}

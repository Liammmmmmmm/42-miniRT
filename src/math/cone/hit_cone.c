/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:49:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 14:40:19 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	get_cone_lateral_uv(t_hit_record *rec, t_cone *cone)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, cone->position);

	if (fabs(cone->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, cone->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, cone->orientation));
	rec->u = clamp_double((atan2(vec3_dot(rel, vec3_cross(cone->orientation, \
		right)), vec3_dot(rel, right)) + PI_D) / (2.0 * PI_D));
	rec->v = clamp_double((vec3_dot(vec3_subtract(rec->point, cone->position),
					cone->orientation) / (cone->height)));
	if (rec->mat && rec->mat->scale != 1)
	{
		rec->u *= rec->mat->scale;
		rec->v *= rec->mat->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
}

static inline void	get_cone_cap_uv(t_hit_record *rec, t_cone *cone)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, vec3_add(cone->position, \
		vec3_multiply_scalar(cone->orientation, cone->height * 0.5)));

	if (fabs(cone->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, cone->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, cone->orientation));
	rec->u = clamp_double((vec3_dot(rel, right) / cone->diameter) + 0.5);
	rec->v = clamp_double((vec3_dot(rel, vec3_cross(cone->orientation, right))
				/ cone->diameter) + 0.5);
	if (rec->mat && rec->mat->scale != 1)
	{
		rec->u *= rec->mat->scale;
		rec->v *= rec->mat->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
}

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
	rec->mat = cone->material;
	get_cone_lateral_uv(rec, cone);
	return (1);
}

static inline char	cone_cap(t_cone *cone, t_ray *r, t_hit_record *rec)
{
	const t_vec3	cap_c = vec3_add(cone->position, \
		vec3_multiply_scalar(vec3_unit(cone->orientation), cone->height));
	const t_vec3	normal = vec3_unit(cone->orientation);
	const double	denom = vec3_dot(r->dir, normal);
	double			t;
	t_vec3			hit_point;

	if (fabs(denom) < 1e-6)
		return (0);
	t = vec3_dot(vec3_subtract(cap_c, r->orig), normal) / denom;
	if (t < IT_MIN || t > IT_MAX)
		return (0);
	hit_point = ray_at(*r, t);
	if (vec3_length(vec3_subtract(hit_point, cap_c)) > (cone->diameter * 0.5))
		return (0);
	rec->t = t;
	rec->point = hit_point;
	rec->normal = normal;
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->part = TOP_CAP;
	rec->mat = cone->material_top;
	if (rec->mat == NULL)
		rec->mat = cone->material;
	get_cone_cap_uv(rec, cone);
	return (1);
}

char	hit_cone(t_cone *cone, t_ray *r, t_hit_record *rec)
{
	t_quadratic	q;
	int			hit_cap;

	if (cone->orientation.x == 0 && cone->orientation.y == 0 && \
		cone->orientation.z == 0)
		cone->orientation.y = 1;
	if (!init_cone_quadratic(&q, cone, r) || !valid_t(&q))
		return (0);
	if (handle_cone_hit(cone, r, rec, &q))
		return (1);
	hit_cap = cone_cap(cone, r, rec);
	if (!hit_cap)
		return (0);
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->normal = set_normal_face(r, &rec->normal, rec);
	return (1);
}

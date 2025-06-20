/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:54:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 14:41:10 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

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
	rec->mat = cyl->material;
	get_cylinder_lateral_uv(rec, cyl);
	return (1);
}

static inline char	cylinder_cap_bottom(t_ray *r, t_cylinder *cyl,
	t_hit_record *rec)
{
	const t_vec3	plane_origin = vec3_subtract(cyl->position, \
		vec3_multiply_scalar(vec3_unit(cyl->orientation), cyl->height * 0.5));
	const double	d = vec3_dot(r->dir, cyl->orientation);
	double			t;
	t_vec3			hit_point;

	if (fabs(d) < 1e-6)
		return (0);
	t = -vec3_dot(vec3_subtract(r->orig, plane_origin), cyl->orientation) / d;
	if (t < IT_MIN || t > IT_MAX)
		return (0);
	hit_point = ray_at(*r, t);
	if (vec3_length(vec3_subtract(hit_point, plane_origin)) > \
			cyl->diameter * 0.5)
		return (0);
	rec->t = t;
	rec->point = hit_point;
	rec->normal = vec3_negate(cyl->orientation);
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->mat = cyl->material_bot;
	if (rec->mat == NULL)
		rec->mat = cyl->material;
	rec->part = BOTTOM_CAP;
	get_cylinder_cap_uv(rec, cyl, -1);
	return (1);
}

static inline char	cylinder_cap_top(t_ray *r, t_cylinder *cyl,
	t_hit_record *rec)
{
	const t_vec3	pl_o = vec3_add(cyl->position, \
		vec3_multiply_scalar(vec3_unit(cyl->orientation), cyl->height * 0.5));
	const double	denom = vec3_dot(r->dir, cyl->orientation);
	t_vec3			hit_point;
	double			t;

	if (fabs(denom) < 1e-6)
		return (0);
	t = -vec3_dot(vec3_subtract(r->orig, pl_o),
			cyl->orientation) / denom;
	if (t < IT_MIN || t > IT_MAX)
		return (0);
	hit_point = ray_at(*r, t);
	if (vec3_length(vec3_subtract(hit_point, pl_o)) > cyl->diameter * 0.5)
		return (0);
	rec->t = t;
	rec->point = hit_point;
	rec->normal = cyl->orientation;
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->mat = cyl->material_top;
	if (rec->mat == NULL)
		rec->mat = cyl->material;
	rec->part = TOP_CAP;
	get_cylinder_cap_uv(rec, cyl, 1);
	return (1);
}

static inline char	init_axe_value(t_cylinder *cyl)
{
	if (cyl->orientation.x == 0 && cyl->orientation.y == 0 && \
		cyl->orientation.z == 0)
		cyl->orientation.y = 1;
	return (0);
}

char	hit_cylinder(t_cylinder *cyl, t_ray *r,	t_hit_record *rec)
{
	t_quadratic		q;
	t_hit_record	tmp_rec;
	char			hit_any;

	hit_any = init_axe_value(cyl);
	if (init_cylinder_quadratic(&q, cyl, r) && valid_t(&q))
		if (handle_cylinder_hit(cyl, r, rec, &q))
			hit_any = 1;
	if (cylinder_cap_bottom(r, cyl, &tmp_rec) && \
		(!hit_any || tmp_rec.t < rec->t))
	{
		*rec = tmp_rec;
		hit_any = 1;
	}
	if (cylinder_cap_top(r, cyl, &tmp_rec) && \
		(!hit_any || tmp_rec.t < rec->t))
	{
		*rec = tmp_rec;
		hit_any = 1;
	}
	if (!hit_any)
		return (0);
	rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
	rec->normal = set_normal_face(r, &rec->normal, rec);
	return (1);
}

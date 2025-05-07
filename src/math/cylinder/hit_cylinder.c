/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:54:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 15:06:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static inline void	get_cylinder_lateral_uv(t_hit_record *rec, t_cylinder *cyl)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, cyl->position);

	if (fabs(cyl->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, cyl->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, cyl->orientation));
	rec->u = clamp_double((atan2(vec3_dot(rel, vec3_cross(cyl->orientation, \
		right)), vec3_dot(rel, right)) + PI_D) / (2.0 * PI_D));

	rec->v = clamp_double((vec3_dot(vec3_subtract(rec->point, cyl->position), cyl->orientation) / (cyl->height)) + 0.5);
	if (rec->mat && rec->mat->scale != 1)
	{
		rec->u *= rec->mat->scale;
		rec->v *= rec->mat->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
}

static inline void get_cylinder_cap_uv(t_hit_record *rec, t_cylinder *cyl, int top)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, vec3_add(cyl->position, \
		vec3_multiply_scalar(cyl->orientation, cyl->height * 0.5 * top)));

	if (fabs(cyl->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, cyl->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, cyl->orientation));
	rec->u = clamp_double((vec3_dot(rel, right) / cyl->diameter) + 0.5);
	rec->v = clamp_double((vec3_dot(rel, vec3_cross(cyl->orientation, right)) / cyl->diameter) + 0.5);
	if (rec->mat && rec->mat->scale != 1)
	{
		rec->u *= rec->mat->scale;
		rec->v *= rec->mat->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
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
	rec->mat = cyl->material;
	get_cylinder_lateral_uv(rec, cyl);
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
	rec->mat = cyl->material_bot;
	if (rec->mat == NULL)
		rec->mat = cyl->material;
	rec->part = BOTTOM_CAP;
	get_cylinder_cap_uv(rec, cyl, -1);
	return (1);
}

static inline char	cylinder_cap_top(t_ray *r, t_cylinder *cyl,
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
	rec->mat = cyl->material_top;
	if (rec->mat == NULL)
		rec->mat = cyl->material;
	rec->part = TOP_CAP;
	get_cylinder_cap_uv(rec, cyl, 1);
	return (1);
}

char	hit_cylinder(t_cylinder *cyl, t_ray *r, t_interval interval,
	t_hit_record *rec)
{
	t_quadratic	q;
	int			hit_cap_bottom;
	int			hit_cap_top;

	rec->u = 0;
	rec->v = 0;
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

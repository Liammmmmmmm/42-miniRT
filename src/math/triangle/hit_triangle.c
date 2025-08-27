/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_triangle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:27:26 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/27 11:59:30 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	interpolate_normal(t_hit_record *rec, t_triangle *t, float u,
	float v)
{
	t_vec3	n;
	float	w;

	w = 1.0f - u - v;
	n.x = w * t->v0.normal.x + u * t->v1.normal.x + v * t->v2.normal.x;
	n.y = w * t->v0.normal.y + u * t->v1.normal.y + v * t->v2.normal.y;
	n.z = w * t->v0.normal.z + u * t->v1.normal.z + v * t->v2.normal.z;
	rec->normal = vec3_unit(n);
}

static int	is_parallel(t_moller *m, char one_sided)
{
	float	a;

	a = vec3_dot(m->e1, m->h);
	if (one_sided == 1)
	{
		if (a < 1e-6)
			return (1);
	}
	else
	{
		if (fabsf(a) < 1e-6)
			return (1);
	}
	m->f = 1.0f / a;
	return (0);
}

static int	get_uv(t_moller *m, t_ray *r, t_triangle *t)
{
	m->s = vec3_subtract(r->orig, t->v0.pos);
	m->u = m->f * vec3_dot(m->s, m->h);
	if (m->u < 0.0f || m->u > 1.0f)
		return (0);
	m->q = vec3_cross(m->s, m->e1);
	m->v = m->f * vec3_dot(r->dir, m->q);
	if (m->v < 0.0f || m->u + m->v > 1.0f)
		return (0);
	return (1);
}

static int	compute_t(t_moller *m)
{
	m->t = m->f * vec3_dot(m->e2, m->q);
	if (m->t < IT_MIN || m->t > IT_MAX)
		return (0);
	return (1);
}

char    hit_triangle(t_triangle *t, t_ray *r, t_hit_record *rec)
{
    t_moller    m;
    t_vec3      outward_normal;

    m.e1 = vec3_subtract(t->v1.pos, t->v0.pos);
    m.e2 = vec3_subtract(t->v2.pos, t->v0.pos);
    m.h = vec3_cross(r->dir, m.e2);
    if (is_parallel(&m, rec->one_sided))
        return (0);
    if (!get_uv(&m, r, t))
        return (0);
    if (!compute_t(&m))
        return (0);
    rec->t = m.t;
    rec->point = ray_at(*r, m.t);
    interpolate_normal(rec, t, m.u, m.v);
    outward_normal = rec->normal;
    if (!rec->one_sided && vec3_dot(r->dir, outward_normal) > 0.0f)
    {
        rec->front_face = 0;
        rec->normal = vec3_multiply_scalar(outward_normal, -1.0f);
    }
    else
        rec->front_face = 1;
    rec->u = (1.0f - m.u - m.v) * t->v0.u + m.u * t->v1.u + m.v * t->v2.u;
    rec->v = (1.0f - m.u - m.v) * t->v0.v + m.u * t->v1.v + m.v * t->v2.v;
    return (1);
}

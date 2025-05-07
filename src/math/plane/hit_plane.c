/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:53:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 12:47:27 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

static inline void	get_uv_plane(t_hit_record *rec, t_plane *p)
{
	t_vec3	up;
	t_vec3	tangent;
	t_vec3	local;
	double	u;
	double	v;

	if (fabs(p->normal.y) < 0.99)
		up = (t_vec3){0, 1, 0};
	else
		up = (t_vec3){1, 0, 0};
	tangent = vec3_unit(vec3_cross(up, p->normal));
	local = vec3_subtract(rec->point, p->position);
	u = vec3_dot(local, tangent);
	v = vec3_dot(local, vec3_cross(p->normal, tangent));
	if (p->material)
	{
		u *= p->material->scale * 0.1;
		v *= p->material->scale * 0.1;
	}
	rec->u = u - floor(u);
	rec->v = v - floor(v);
}

char	hit_plane(t_plane *p, const t_ray *r, t_interval i, t_hit_record *rec)
{
	const double	denom = p->normal.x * r->dir.x + p->normal.y * r->dir.y + \
		p->normal.z * r->dir.z;
	const double	t = ((p->position.x - r->orig.x) * p->normal.x + \
					(p->position.y - r->orig.y) * p->normal.y + \
					(p->position.z - r->orig.z) * p->normal.z) / denom;

	if (fabs(denom) < 1e-8)
		return (0);
	if (t < i.min || t > i.max)
		return (0);
	rec->t = t;
	rec->point.x = r->orig.x + t * r->dir.x;
	rec->point.y = r->orig.y + t * r->dir.y;
	rec->point.z = r->orig.z + t * r->dir.z;
	rec->front_face = ((r->dir.x * p->normal.x + r->dir.y * p->normal.y + \
			r->dir.z * p->normal.z) < 0);
	if (rec->front_face)
		rec->normal = p->normal;
	else
		rec->normal = (t_vec3){-p->normal.x, -p->normal.y, -p->normal.z};
	get_uv_plane(rec, p);
	return (1);
}

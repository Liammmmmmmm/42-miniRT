/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:53:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 18:32:18 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

/*
char	hit_plane(t_plane *plane, const t_ray *r, \
	t_interval interval, t_hit_record *rec)
{
	register double	t;

	t = vec3_dot(vec3_subtract(plane->position, r->orig), plane->normal) / \
			vec3_dot(plane->normal, r->dir);
	if (t >= interval.min && t <= interval.max)
	{
		rec->t = t;
		rec->point = ray_at(*r, rec->t);
		rec->normal = plane->normal;
		if (vec3_dot(r->dir, rec->normal) > 0)
			return (0);
		rec->normal = vec3_unit(rec->normal);
		rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
		return (1);
	}
	return (0);
}
*/
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
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:53:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 20:24:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

t_vec3	vec3_subtract(register t_vec3 u, register t_vec3 v);

char	hit_plane(const t_vec3 point_on_plane, const t_vec3 normal, \
	const t_ray *r, t_interval interval, t_hit_record *rec)
{
	double	denom;
	double	t;
	t_vec3	oc;

	denom = vec3_dot(normal, r->dir);
	oc = vec3_subtract(point_on_plane, r->orig);
	t = vec3_dot(oc, normal) / denom;
	if (t >= interval.min && t <= interval.max)
	{
		rec->t = t;
		rec->point = ray_at(*r, rec->t);
		rec->normal = normal;
		if (vec3_dot(r->dir, rec->normal) > 0)
			return (0);
		rec->normal = vec3_unit(rec->normal);
		rec->front_face = (vec3_dot(r->dir, rec->normal) < 0);
		return (1);
	}
	return (0);
}

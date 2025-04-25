/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:53:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 17:03:54 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

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

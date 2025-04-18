/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:53:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/18 13:48:58 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

char	hit_plane(const t_vec3 point_on_plane, const t_vec3 normal, \
	const t_ray *r, t_interval interval, t_hit_record *rec)
{
	register double	t;

	t = vec3_dot(vec3_subtract(point_on_plane, r->orig), normal) / vec3_dot(normal, r->dir);
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

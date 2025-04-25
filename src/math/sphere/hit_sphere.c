/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:47:38 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 18:25:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline double	clamp(double v)
{
	if (v < 0)
		return (0);
	else if (v > 1)
		return (1);
	return (v);
}

static inline void	get_sphere_uv(t_hit_record *rec, t_sphere *sphere)
{
	rec->u = clamp((atan2((-rec->point.z + sphere->position.z)
					/ sphere->radius, (rec->point.x - sphere->position.x)
					/ sphere->radius) + PI_D) / (2 * PI_D));
	rec->v = clamp(acos((-rec->point.y + sphere->position.y)
				/ sphere->radius) / PI_D);
	rec->u *= 4;
	rec->v *= 4;
	while (rec->u > 1)
		rec->u -= 1;
	while (rec->v > 1)
		rec->v -= 1;
}

char	hit_sphere(t_sphere *sp, t_ray *r, t_interval i, t_hit_record *rec)
{
	t_quadratic	q;
	t_vec3		outward;

	if (!init_sphere_quadratic(&q, sp, r) || !valid_t(&q, i))
		return (0);
	rec->t = q.t_hit;
	rec->point = ray_at(*r, rec->t);
	outward = vec3_divide_scalar(
			vec3_subtract(rec->point, sp->position),
			sp->radius
			);
	rec->normal = set_normal_face(r, &outward, rec);
	get_sphere_uv(rec, sp);
	return (1);
}

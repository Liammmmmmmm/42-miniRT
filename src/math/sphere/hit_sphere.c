/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:47:38 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/05 16:26:36 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	get_sphere_uv(t_hit_record *rec, t_sphere *sphere)
{
	rec->u = clamp_double((atan2((-rec->point.z + sphere->position.z)
					/ sphere->radius, (rec->point.x - sphere->position.x)
					/ sphere->radius) + PI_D) / (2 * PI_D));
	rec->v = clamp_double(acos((-rec->point.y + sphere->position.y)
				/ sphere->radius) / PI_D);
	if (sphere->material && sphere->material->scale != 1)
	{
		rec->u *= sphere->material->scale;
		rec->v *= sphere->material->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:17:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/18 11:31:09 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline double clamp(double v)
{
	if (v < 0)
		return (0);
	else if (v > 1)
		return (1);
	return (v);
}

static inline void get_sphere_uv(t_hit_record *rec, t_sphere *sphere)
{
	rec->u = clamp((atan2((-rec->point.z + sphere->position.z) / sphere->radius, (rec->point.x - sphere->position.x) / sphere->radius) + PI_D) / (2 * PI_D));
	rec->v = clamp(acos((-rec->point.y + sphere->position.y) / sphere->radius) / PI_D);

	// Truc pour avoir plusieurs repetitions d'une texture sur une sphere
	// rec->u *= 4;
	// rec->v *= 4;
	// while (rec->u > 1)
	// 	rec->u -= 1;
	// while (rec->v > 1)
	// 	rec->v -= 1;
}

char	hit_sphere(t_sphere sphere, t_ray *r, \
	t_interval interval, t_hit_record *rec)
{
	t_vec3			oc;
	register double	a;
	register double	b;
	register double	d;
	register double	sqrtd;
	register double	root;

	oc = (t_vec3){r->orig.x - sphere.position.x, r->orig.y - sphere.position.y, r->orig.z - sphere.position.z};
	a = r->dir.x * r->dir.x + r->dir.y * r->dir.y + r->dir.z * r->dir.z;
	b = r->dir.x * oc.x + r->dir.y * oc.y + r->dir.z * oc.z;
	d = b * b - a * ((oc.x * oc.x + oc.y * oc.y + oc.z * oc.z) - sphere.sqrt_radius);
	if (d < 0)
		return (0);
	sqrtd = sqrt(d);
	root = (-b - sqrtd) / a;
	if (root < interval.min || root > interval.max) 
	{
		root = (-b + sqrtd) / a;
		if (root < interval.min || root > interval.max)
			return (0);
	}
	rec->t = root;
	rec->point = ray_at(*r, rec->t);
	oc = vec3_divide_scalar((t_vec3){rec->point.x - sphere.position.x, rec->point.y - sphere.position.y, rec->point.z - sphere.position.z}, sphere.radius);
	rec->normal = set_normal_face(r, &oc, rec);
	get_sphere_uv(rec, &sphere);
	return (1);
}

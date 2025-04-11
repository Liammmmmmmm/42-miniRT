/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:17:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 21:09:20 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

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
	return (1);
}

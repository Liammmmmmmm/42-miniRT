/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:17:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/23 19:16:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

// char	hit_sphere(const t_vec3 center, float radius, const t_ray *r)
// {
// 	t_vec3	oc;
// 	float	a;
// 	float	b;
// 	float	c;
// 	float	discriminant;

// 	oc = vec3_subtract(r->orig, center);
// 	a = vec3_dot(r->dir, r->dir);
// 	b = -2.0 * vec3_dot(r->dir, oc);
// 	c = vec3_dot(oc, oc) - radius * radius;
// 	discriminant = b * b - 4.0 * a * c;
// 	return (discriminant >= 0);
// }

char hit_sphere(const t_vec3 center, float radius, const t_ray *r, \
	t_interval interval, t_hit_record *rec)
{
	t_vec3	oc;
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	sqrtd;
	float	root;

    oc = vec3_subtract(r->orig, center);
    a = vec3_dot(r->dir, r->dir);
    b = vec3_dot(r->dir, oc);
    c = vec3_dot(oc, oc) - radius * radius;
    discriminant = b * b - a * c;
    if (discriminant < 0)
        return (0);
    sqrtd = sqrt(discriminant);
    root = (-b - sqrtd) / a;
    if (root < interval.min || root > interval.max)
	{
        root = (-b + sqrtd) / a;
        if (root < interval.min || root > interval.max)
            return (0);
	}
	rec->t = root;
    rec->point = ray_at(*r, rec->t);
    oc = vec3_divide_scalar(vec3_subtract(rec->point, center), radius);
	rec->normal = set_normal_face(r, &oc);
    return (1);
}
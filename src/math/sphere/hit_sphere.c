/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:17:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/20 18:30:03 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "math.h"

char	hit_sphere(const t_vec3 *center, double radius, const t_ray *r)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	oc = vec3_subtract(&r->orig, center);
	a = vec3_dot(&r->dir, &r->dir);
	b = -2.0 * vec3_dot(&r->dir, &oc);
	c = vec3_dot(&oc, &oc) - radius * radius;
	discriminant = b * b - 4.0 * a * c;
	return (discriminant >= 0);
}

// double	hit_sphere(const t_vec3 *center, double radius, const t_ray *r)
// {
// 	t_vec3	oc;
// 	double	a;
// 	double	h;
// 	double	c;
// 	double	discriminant;

// 	oc = vec3_subtract(&r->orig, center);
// 	a = vec3_length_squared(&r->dir);
// 	h = vec3_dot(&r->dir, &oc);
// 	c = vec3_length_squared(&oc) - radius * radius;
// 	discriminant = h * h - a * c;
// 	if (discriminant < 0)
// 		return (-1.0);
// 	else
// 		return ((h - sqrt(discriminant)) / a);
// }

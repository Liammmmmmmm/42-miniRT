/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratique_hyperboloid.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:17:52 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/29 15:25:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	get_basis_vectors(t_hyperboloid *h, t_vec3 *x, t_vec3 *y, t_vec3 *z)
{
	t_vec3	temp;

	*z = vec3_unit(h->orientation);
	if (fabs(z->x) > 0.9)
		temp = (t_vec3){0, 1, 0};
	else
		temp = (t_vec3){1, 0, 0};
	*x = vec3_unit(vec3_cross(temp, *z));
	*y = vec3_cross(*z, *x);
}

static inline t_vec3	to_local(t_vec3 v, t_vec3 x, t_vec3 y, t_vec3 z)
{
	return ((t_vec3){
		.x = vec3_dot(v, x),
		.y = vec3_dot(v, y),
		.z = vec3_dot(v, z)
	});
}

static void	compute_quadratic_coeffs(t_quadratic *q, t_vec3 dir, t_vec3 ori, \
	t_hyperboloid *h)
{
	const double	aa = h->a * h->a;
	const double	bb = h->b * h->b;
	const double	cc = h->c * h->c;

	q->a = (dir.x * dir.x) / aa + (dir.y * dir.y) / bb - (dir.z * dir.z) \
		/ cc;
	q->b = 2.0 * ((dir.x * ori.x) / aa + (dir.y * ori.y) / bb \
		- (dir.z * ori.z) / cc);
	q->c = (ori.x * ori.x) / aa + (ori.y * ori.y) / bb - (ori.z * ori.z) / cc \
		- h->shape;
}

inline char	init_hyperboloid_quadratic(t_quadratic *q, t_hyperboloid *h, \
	t_ray *r)
{
	t_vec3	x;
	t_vec3	y;
	t_vec3	z;

	get_basis_vectors(h, &x, &y, &z);
	compute_quadratic_coeffs(q, to_local(r->dir, x, y, z), \
		to_local(vec3_subtract(r->orig, h->position), x, y, z), h);
	return (solve_quadratic(q));
}

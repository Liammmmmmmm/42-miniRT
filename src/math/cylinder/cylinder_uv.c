/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:54:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 15:06:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"
#include <math.h>

inline void	get_cylinder_lateral_uv(t_hit_record *rec, t_cylinder *cyl)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, cyl->position);

	if (fabs(cyl->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, cyl->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, cyl->orientation));
	rec->u = clamp_double((atan2(vec3_dot(rel, vec3_cross(cyl->orientation, \
		right)), vec3_dot(rel, right)) + PI_D) / (2.0 * PI_D));
	rec->v = clamp_double((vec3_dot(vec3_subtract(rec->point, cyl->position),
					cyl->orientation) / (cyl->height)) + 0.5);
	if (rec->mat && rec->mat->scale != 1)
	{
		rec->u *= rec->mat->scale;
		rec->v *= rec->mat->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
}

inline void	get_cylinder_cap_uv(t_hit_record *rec, t_cylinder *cyl, int top)
{
	t_vec3			right;
	const t_vec3	rel = vec3_subtract(rec->point, vec3_add(cyl->position, \
		vec3_multiply_scalar(cyl->orientation, cyl->height * 0.5 * top)));

	if (fabs(cyl->orientation.y) < 0.99)
		right = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, cyl->orientation));
	else
		right = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, cyl->orientation));
	rec->u = clamp_double((vec3_dot(rel, right) / cyl->diameter) + 0.5);
	rec->v = clamp_double((vec3_dot(rel, vec3_cross(cyl->orientation, right))
				/ cyl->diameter) + 0.5);
	if (rec->mat && rec->mat->scale != 1)
	{
		rec->u *= rec->mat->scale;
		rec->v *= rec->mat->scale;
		rec->u = modf(rec->u, &(double){0});
		rec->v = modf(rec->v, &(double){0});
	}
}

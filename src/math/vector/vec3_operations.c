/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:44:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/20 18:07:40 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

inline t_vec3	vec3_add(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){u->x + v->x, u->y + v->y, u->z + v->z});
}

inline t_vec3	vec3_subtract(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){u->x - v->x, u->y - v->y, u->z - v->z});
}

inline t_vec3	vec3_multiply(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){u->x * v->x, u->y * v->y, u->z * v->z});
}

inline t_vec3	vec3_multiply_scalar(const t_vec3 *v, double t)
{
	return ((t_vec3){v->x * t, v->y * t, v->z * t});
}

inline t_vec3	vec3_divide_scalar(const t_vec3 *v, double t)
{
	if (t != 0.0)
		return ((t_vec3){v->x / t, v->y / t, v->z / t});
	return ((t_vec3){0.0, 0.0, 0.0});
}

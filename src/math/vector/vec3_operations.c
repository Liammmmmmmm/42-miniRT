/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:44:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/21 12:13:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

t_vec3	vec3_add(const t_vec3 u, const t_vec3 v)
{
	return ((t_vec3){u.x + v.x, u.y + v.y, u.z + v.z});
}

t_vec3	vec3_subtract(const t_vec3 u, const t_vec3 v)
{
	return ((t_vec3){u.x - v.x, u.y - v.y, u.z - v.z});
}

t_vec3	vec3_multiply(const t_vec3 u, const t_vec3 v)
{
	return ((t_vec3){u.x * v.x, u.y * v.y, u.z * v.z});
}

t_vec3	vec3_multiply_scalar(const t_vec3 v, double t)
{
	return ((t_vec3){v.x * t, v.y * t, v.z * t});
}

t_vec3	vec3_divide_scalar(const t_vec3 v, double t)
{
	if (t != 0.0)
		return ((t_vec3){v.x / t, v.y / t, v.z / t});
	return ((t_vec3){0.0, 0.0, 0.0});
}

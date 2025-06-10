/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operation_scalar.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:45:57 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/10 18:41:29 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "structs.h"
#include <math.h>

inline t_vec3	vec3_multiply_scalar(const t_vec3 v, double t)
{
	return ((t_vec3){v.x * t, v.y * t, v.z * t});
}

inline t_vec3	vec3_divide_scalar(const t_vec3 v, double t)
{
	if (t != 0.0)
		return ((t_vec3){v.x / t, v.y / t, v.z / t});
	return ((t_vec3){0.0, 0.0, 0.0});
}

inline t_vec3	vec3_divide_safe(t_vec3 a, t_vec3 b, double	min)
{
	t_vec3	result;

	if (fabs(b.x) < 0.00001f)
		result.x = min;
	else
		result.x = a.x / b.x;
	if (fabs(b.y) < 0.00001f)
		result.y = min;
	else
		result.y = a.y / b.y;
	if (fabs(b.z) < 0.00001f)
		result.z = min;
	else
		result.z = a.z / b.z;
	return (result);
}

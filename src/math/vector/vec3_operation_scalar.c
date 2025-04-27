/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operation_scalar.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:45:57 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/27 17:42:26 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "structs.h"

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

inline t_vec3	vec3_subtract_scalar(t_vec3 vec, double t)
{
	return ((t_vec3){vec.x -= t, vec.y -= t, vec.z -= t});
}

inline t_vec3	vec3_add_scalar(t_vec3 vec, double t)
{
	return ((t_vec3){vec.x += t, vec.y += t, vec.z += t});
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:44:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/10 16:56:48 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

inline t_vec3	vec3_add(const t_vec3 u, const t_vec3 v)
{
	return ((t_vec3){u.x + v.x, u.y + v.y, u.z + v.z});
}

inline t_vec3	vec3_subtract(register t_vec3 u, register t_vec3 v)
{
	return ((t_vec3){u.x - v.x, u.y - v.y, u.z - v.z});
}

inline t_vec3	vec3_multiply(const t_vec3 u, const t_vec3 v)
{
	return ((t_vec3){u.x * v.x, u.y * v.y, u.z * v.z});
}

inline t_vec3	vec3_divide(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x / b.x, a.y / b.y, a.z / b.z});
}

inline t_vec3	vec3_sqrt(t_vec3 v)
{
	return ((t_vec3){sqrt(v.x), sqrt(v.y), sqrt(v.z)});
}

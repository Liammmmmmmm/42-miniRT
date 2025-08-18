/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:44:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/18 16:56:19 by lilefebv         ###   ########lyon.fr   */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_dot_cross.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:49:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/20 14:26:19 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

inline double	vec3_dot(register t_vec3 u, register t_vec3 v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

inline t_vec3	vec3_cross(const t_vec3 u, const t_vec3 v)
{
	return ((t_vec3){
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	});
}

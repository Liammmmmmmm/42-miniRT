/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:50:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 19:36:40 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

inline t_vec3	vec3_negate(const t_vec3 v)
{
	return ((t_vec3){-v.x, -v.y, -v.z});
}

inline t_vec3	vec3_unit(const t_vec3 v)
{
	double	length;

	length = vec3_length(v);
	if (length > 0.0)
		return (vec3_divide_scalar(v, length));
	return ((t_vec3){0.0, 0.0, 0.0});
}

inline t_vec3	vec3_init(double x, double y, double z)
{
	return ((t_vec3){x, y, z});
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:50:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/23 17:43:39 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

t_vec3	vec3_negate(const t_vec3 v)
{
	return ((t_vec3){-v.x, -v.y, -v.z});
}

t_vec3	vec3_unit(const t_vec3 v)
{
	double	length;

	length = vec3_length(v);
	if (length > 0.0)
		return (vec3_divide_scalar(v, length));
	return ((t_vec3){0.0, 0.0, 0.0});
}

t_vec3	vec3_init(float x, float y, float z)
{
	return ((t_vec3){x, y, z});
}

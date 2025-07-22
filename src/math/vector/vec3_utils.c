/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:50:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/22 15:30:13 by lilefebv         ###   ########lyon.fr   */
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

inline t_vec3	vec3_fmax(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){ft_dmax(a.x, b.x), ft_dmax(a.y, b.y), ft_dmax(a.z, b.z)});
}

inline t_vec3	vec3_fmin(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){ft_dmin(a.x, b.x), ft_dmin(a.y, b.y), ft_dmin(a.z, b.z)});
}

inline t_vec3	vec3_rotate(t_vec3 v, t_vec3 axis, float angle)
{
	t_vec3	rotated_v;
	float	cos_a;
	float	sin_a;
	float	dot_product;

	cos_a = cos(angle);
	sin_a = sin(angle);
	dot_product = vec3_dot(v, axis);
	rotated_v.x = v.x * cos_a + (axis.y * v.z - axis.z * v.y) * sin_a
		+ axis.x * dot_product * (1 - cos_a);
	rotated_v.y = v.y * cos_a + (axis.z * v.x - axis.x * v.z) * sin_a
		+ axis.y * dot_product * (1 - cos_a);
	rotated_v.z = v.z * cos_a + (axis.x * v.y - axis.y * v.x) * sin_a
		+ axis.z * dot_product * (1 - cos_a);
	return (rotated_v);
}

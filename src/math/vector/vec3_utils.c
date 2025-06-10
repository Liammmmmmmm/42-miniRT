/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:50:11 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/10 18:45:23 by madelvin         ###   ########.fr       */
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

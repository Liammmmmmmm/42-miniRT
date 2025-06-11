/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_simd_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:05:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/11 13:22:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "vec4.h"

inline t_vec4	vec3_to_vec4(t_vec3 vec3)
{
	return ((t_vec4){_mm256_set_pd(0.0, vec3.z, vec3.y, vec3.x)});
}

inline t_vec3	vec4_to_vec3(t_vec4 v)
{
	double	vals[4];

	_mm256_storeu_pd(vals, v.data);
	return ((t_vec3){vals[0], vals[1], vals[2]});
}

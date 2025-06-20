/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:12:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 18:59:39 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "vec4.h"

inline double	vec4_length(t_vec4 v)
{
	double	tmp[4];

	_mm256_storeu_pd(tmp, _mm256_mul_pd(v.data, v.data));
	return (sqrt(tmp[0] + tmp[1] + tmp[2]));
}

inline t_vec4	vec4_unit(t_vec4 v)
{
	const double	len = vec4_length(v);

	if (len > 0.0)
		return (vec4_divide_scalar(v, len));
	return ((t_vec4){_mm256_setzero_pd()});
}

inline t_vec4	vec4_negate(t_vec4 v)
{
	return ((t_vec4){_mm256_mul_pd(v.data, _mm256_set1_pd(-1))});
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_scalar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:59:32 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 18:59:31 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "vec4.h"

inline t_vec4	vec4_multiply_scalar(t_vec4 v, double t)
{
	return ((t_vec4){_mm256_mul_pd(v.data, _mm256_set1_pd(t))});
}

inline t_vec4	vec4_divide_scalar(t_vec4 v, double t)
{
	if (t == 0.0)
		return ((t_vec4){_mm256_setzero_pd()});
	return ((t_vec4){_mm256_div_pd(v.data, _mm256_set1_pd(t))});
}

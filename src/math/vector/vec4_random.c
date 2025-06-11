/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_random.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:35:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 13:22:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>
#include "vec4.h"

t_vec4	vec4_random(void)
{
	return ((t_vec4){_mm256_set_pd(0.0, random_double(), random_double(), random_double())});
}

t_vec4	vec4_random_in_interval(double min, double max)
{
	return ((t_vec4){_mm256_set_pd(0.0,
		random_double_in_interval(min, max),
		random_double_in_interval(min, max),
		random_double_in_interval(min, max)
	)});
}

t_vec4	vec4_random_unit(void)
{
	t_vec3	p;
	double	lensq;

	while (1)
	{
		p = vec3_random_in_interval(-1, 1);
		lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <= 1.0)
			return (vec4_divide_scalar(vec3_to_vec4(p), sqrt(lensq)));
	}
}

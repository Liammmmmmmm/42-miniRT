/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_random.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:35:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 19:17:13 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

t_vec3	vec3_random(uint64_t *rand)
{
	return ((t_vec3){random_double(rand), random_double(rand),
			random_double(rand)});
}

t_vec3	vec3_random_in_interval(double min, double max, uint64_t *rand)
{
	return ((t_vec3){
		random_double_in_interval(min, max, rand),
		random_double_in_interval(min, max, rand),
		random_double_in_interval(min, max, rand)
	});
}

t_vec3	vec3_random_unit(uint64_t *rand)
{
	t_vec3	p;
	double	lensq;

	while (1)
	{
		p = vec3_random_in_interval(-1, 1, rand);
		lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <= 1.0)
			return (vec3_divide_scalar(p, sqrt(lensq)));
	}
}

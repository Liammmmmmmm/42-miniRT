/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_random.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:35:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 20:23:55 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

t_vec3	vec3_random(void)
{
	return ((t_vec3){random_double(), random_double(), random_double()});
}

t_vec3	vec3_random_in_interval(double min, double max)
{
	return ((t_vec3){
		random_double_in_interval(min, max),
		random_double_in_interval(min, max),
		random_double_in_interval(min, max)
	});
}

t_vec3	vec3_random_unit(void)
{
	t_vec3	p;
	double	lensq;

	while (1)
	{
		p = vec3_random_in_interval(-1, 1);
		lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <= 1.0)
			return (vec3_divide_scalar(p, sqrt(lensq)));
	}
}

t_vec3	vec3_random_on_hemisphere(const t_vec3 normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = vec3_random_unit();
	if (vec3_dot(on_unit_sphere, normal) >= 0.0)
		return (on_unit_sphere);
	return (vec3_negate(on_unit_sphere));
}

t_vec3	vec3_random_sample(void)
{
	return ((t_vec3){random_double() - 0.5, random_double() - 0.5, 0});
}

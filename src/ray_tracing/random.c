/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:32:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 14:16:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

double	random_double()
{
	return (rand() / (RAND_MAX + 1.0));
}

double	random_double_in_interval(double min, double max)
{
	return (min + (rand() / ((double)RAND_MAX / (max - min))));
}

t_vec3	random_vec3()
{
	return ((t_vec3){random_double(), random_double(), random_double()});
}

t_vec3	random_vec3_in_interval(double min, double max)
{
	return ((t_vec3){
		random_double_in_interval(min, max),
		random_double_in_interval(min, max),
		random_double_in_interval(min, max)
	});
}

t_vec3	random_vec3_unit()
{
	t_vec3	p;
	double	lensq;

	while (1)
	{
		p = random_vec3_in_interval(-1, 1);
		lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <= 1.0)
			return (vec3_divide_scalar(p, sqrt(lensq)));
	}
}

t_vec3	random_on_hemisphere(const t_vec3 normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = random_vec3_unit();
	if (vec3_dot(on_unit_sphere, normal) >= 0.0)
		return (on_unit_sphere);
	return(vec3_negate(on_unit_sphere));
}
